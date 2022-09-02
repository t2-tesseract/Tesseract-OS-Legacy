#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "Fat.h"

#define SECTOR_SIZE             512
#define MAX_PATH_SIZE           256
#define MAX_FILE_HANDLES        10
#define ROOT_DIRECTORY_HANDLE   -1

typedef struct 
{
    uint8_t BootJumpInstruction[3];
    uint8_t OemIdentifier[8];
    uint16_t BytesPerSector;
    uint8_t SectorsPerCluster;
    uint16_t ReservedSectors;
    uint8_t FatCount;
    uint16_t DirEntryCount;
    uint16_t TotalSectors;
    uint8_t MediaDescriptorType;
    uint16_t SectorsPerFat;
    uint16_t SectorsPerTrack;
    uint16_t Heads;
    uint32_t HiddenSectors;
    uint32_t LargeSectorCount;

    // extended boot record
    uint8_t DriveNumber;
    uint8_t _Reserved;
    uint8_t Signature;
    uint32_t VolumeId;          // serial number, value doesn't matter
    uint8_t VolumeLabel[11];    // 11 bytes, padded with spaces
    uint8_t SystemId[8];

    // ... we don't care about code ...

} __attribute__((packed)) FatBootSector;


typedef struct
{
    uint8_t Buffer[SECTOR_SIZE];
    FatFile Public;
    bool Opened;
    uint32_t FirstCluster;
    uint32_t CurrentCluster;
    uint32_t CurrentSectorInCluster;

} FatFileData;

typedef struct
{
    union
    {
        FatBootSector BootSector;
        uint8_t BootSectorBytes[SECTOR_SIZE];
    } BS;

    FatFileData RootDirectory;

    FatFileData OpenedFiles[MAX_FILE_HANDLES];

} FatData;

static FatData* g_Data;
static uint8_t* g_Fat = NULL;
static uint32_t g_DataSectionLba;


bool FatReadBootSector(DISK* disk)
{
    return DiskReadSectors(disk, 0, 1, g_Data->BS.BootSectorBytes);
}

bool FatReadFat(DISK* disk)
{
    return DiskReadSectors(disk, g_Data->BS.BootSector.ReservedSectors, g_Data->BS.BootSector.SectorsPerFat, g_Fat);
}

bool FatInitialize(DISK* disk)
{
    g_Data = (FatData*)MEMORY_FAT_ADDR;

    // read boot sector
    if (!FatReadBootSector(disk))
    {
        Printk("FAT: read boot sector failed\r\n");
        return false;
    }

    // read FAT
    g_Fat = (uint8_t*)g_Data + sizeof(FatData);
    uint32_t fatSize = g_Data->BS.BootSector.BytesPerSector * g_Data->BS.BootSector.SectorsPerFat;
    if (sizeof(FatData) + fatSize >= MEMORY_FAT_SIZE)
    {
        Printk("FAT: not enough memory to read FAT! Required %lu, only have %u\r\n", sizeof(FatData) + fatSize, MEMORY_FAT_SIZE);
        return false;
    }

    if (!FatReadFat(disk))
    {
        Printk("FAT: read FAT failed\r\n");
        return false;
    }

    // open root directory file
    uint32_t rootDirLba = g_Data->BS.BootSector.ReservedSectors + g_Data->BS.BootSector.SectorsPerFat * g_Data->BS.BootSector.FatCount;
    uint32_t rootDirSize = sizeof(FatDirectoryEntry) * g_Data->BS.BootSector.DirEntryCount;

    g_Data->RootDirectory.Public.Handle = ROOT_DIRECTORY_HANDLE;
    g_Data->RootDirectory.Public.IsDirectory = true;
    g_Data->RootDirectory.Public.Position = 0;
    g_Data->RootDirectory.Public.Size = sizeof(FatDirectoryEntry) * g_Data->BS.BootSector.DirEntryCount;
    g_Data->RootDirectory.Opened = true;
    g_Data->RootDirectory.FirstCluster = rootDirLba;
    g_Data->RootDirectory.CurrentCluster = rootDirLba;
    g_Data->RootDirectory.CurrentSectorInCluster = 0;

    if (!DiskReadSectors(disk, rootDirLba, 1, g_Data->RootDirectory.Buffer))
    {
        Printk("FAT: read root directory failed\r\n");
        return false;
    }

    // calculate data section
    uint32_t rootDirSectors = (rootDirSize + g_Data->BS.BootSector.BytesPerSector - 1) / g_Data->BS.BootSector.BytesPerSector;
    g_DataSectionLba = rootDirLba + rootDirSectors;

    // reset opened files
    for (int i = 0; i < MAX_FILE_HANDLES; i++)
        g_Data->OpenedFiles[i].Opened = false;

    return true;
}

uint32_t FatClusterToLba(uint32_t cluster)
{
    return g_DataSectionLba + (cluster - 2) * g_Data->BS.BootSector.SectorsPerCluster;
}

FatFile* FatOpenEntry(DISK* disk, FatDirectoryEntry* entry)
{
    // find empty handle
    int handle = -1;
    for (int i = 0; i < MAX_FILE_HANDLES && handle < 0; i++)
    {
        if (!g_Data->OpenedFiles[i].Opened)
            handle = i;
    }

    // out of handles
    if (handle < 0)
    {
        Printk("FAT: out of file handles\r\n");
        return false;
    }

    // setup vars
    FatFileData* fd = &g_Data->OpenedFiles[handle];
    fd->Public.Handle = handle;
    fd->Public.IsDirectory = (entry->Attributes & FatATTRIBUTE_DIRECTORY) != 0;
    fd->Public.Position = 0;
    fd->Public.Size = entry->Size;
    fd->FirstCluster = entry->FirstClusterLow + ((uint32_t)entry->FirstClusterHigh << 16);
    fd->CurrentCluster = fd->FirstCluster;
    fd->CurrentSectorInCluster = 0;

    if (!DiskReadSectors(disk, FatClusterToLba(fd->CurrentCluster), 1, fd->Buffer))
    {
        Printk("FAT: open entry failed - read error cluster=%u lba=%u\n", fd->CurrentCluster, FatClusterToLba(fd->CurrentCluster));
        for (int i = 0; i < 11; i++)
            Printk("%c", entry->Name[i]);
        Printk("\n");
        return false;
    }

    fd->Opened = true;
    return &fd->Public;
}

uint32_t FatNextCluster(uint32_t currentCluster)
{    
    uint32_t fatIndex = currentCluster * 3 / 2;

    if (currentCluster % 2 == 0)
        return (*(uint16_t*)(g_Fat + fatIndex)) & 0x0FFF;
    else
        return (*(uint16_t*)(g_Fat + fatIndex)) >> 4;
}

uint32_t FatRead(DISK* disk, FatFile* file, uint32_t byteCount, void* dataOut)
{
    // get file data
    FatFileData* fd = (file->Handle == ROOT_DIRECTORY_HANDLE) 
        ? &g_Data->RootDirectory 
        : &g_Data->OpenedFiles[file->Handle];

    uint8_t* u8DataOut = (uint8_t*)dataOut;

    // don't read past the end of the file
    if (!fd->Public.IsDirectory || (fd->Public.IsDirectory && fd->Public.Size != 0))
        byteCount = min(byteCount, fd->Public.Size - fd->Public.Position);

    while (byteCount > 0)
    {
        uint32_t leftInBuffer = SECTOR_SIZE - (fd->Public.Position % SECTOR_SIZE);
        uint32_t take = min(byteCount, leftInBuffer);

        MemoryCopy(u8DataOut, fd->Buffer + fd->Public.Position % SECTOR_SIZE, take);
        u8DataOut += take;
        fd->Public.Position += take;
        byteCount -= take;

        // Printk("leftInBuffer=%lu take=%lu\r\n", leftInBuffer, take);
        // See if we need to read more data
        if (leftInBuffer == take)
        {
            // Special handling for root directory
            if (fd->Public.Handle == ROOT_DIRECTORY_HANDLE)
            {
                ++fd->CurrentCluster;

                // read next sector
                if (!DiskReadSectors(disk, fd->CurrentCluster, 1, fd->Buffer))
                {
                    Printk("FAT: read error!\r\n");
                    break;
                }
            }
            else
            {
                // calculate next cluster & sector to read
                if (++fd->CurrentSectorInCluster >= g_Data->BS.BootSector.SectorsPerCluster)
                {
                    fd->CurrentSectorInCluster = 0;
                    fd->CurrentCluster = FatNextCluster(fd->CurrentCluster);
                }

                if (fd->CurrentCluster >= 0xFF8)
                {
                    // Mark end of file
                    fd->Public.Size = fd->Public.Position;
                    break;
                }

                // read next sector
                if (!DiskReadSectors(disk, FatClusterToLba(fd->CurrentCluster) + fd->CurrentSectorInCluster, 1, fd->Buffer))
                {
                    Printk("FAT: read error!\r\n");
                    break;
                }
            }
        }
    }

    return u8DataOut - (uint8_t*)dataOut;
}

bool FatReadEntry(DISK* disk, FatFile* file, FatDirectoryEntry* dirEntry)
{
    return FatRead(disk, file, sizeof(FatDirectoryEntry), dirEntry) == sizeof(FatDirectoryEntry);
}

void FatClose(FatFile* file)
{
    if (file->Handle == ROOT_DIRECTORY_HANDLE)
    {
        file->Position = 0;
        g_Data->RootDirectory.CurrentCluster = g_Data->RootDirectory.FirstCluster;
    }
    else
    {
        g_Data->OpenedFiles[file->Handle].Opened = false;
    }
}

bool FatFindFile(DISK* disk, FatFile* file, const char* name, FatDirectoryEntry* entryOut)
{
    char fatName[12];
    FatDirectoryEntry entry;

    // convert from name to fat name
    MemorySet(fatName, ' ', sizeof(fatName));
    fatName[11] = '\0';

    const char* ext = StringChar(name, '.');
    if (ext == NULL)
        ext = name + 11;

    for (int i = 0; i < 8 && name[i] && name + i < ext; i++)
        fatName[i] = Toupper(name[i]);

    if (ext != name + 11)
    {
        for (int i = 0; i < 3 && ext[i + 1]; i++)
            fatName[i + 8] = Toupper(ext[i + 1]);
    }

    while (FatReadEntry(disk, file, &entry))
    {
        if (MemoryCompare(fatName, entry.Name, 11) == 0)
        {
            *entryOut = entry;
            return true;
        }        
    }
    
    return false;
}

FatFile* FatOpen(DISK* disk, const char* path)
{
    char name[MAX_PATH_SIZE];

    // ignore leading slash
    if (path[0] == '/')
        path++;

    FatFile* current = &g_Data->RootDirectory.Public;

    while (*path) {
        // extract next file name from path
        bool isLast = false;
        const char* delim = StringChar(path, '/');
        if (delim != NULL)
        {
            MemoryCopy(name, path, delim - path);
            name[delim - path + 1] = '\0';
            path = delim + 1;
        }
        else
        {
            unsigned len = StringLength(path);
            MemoryCopy(name, path, len);
            name[len + 1] = '\0';
            path += len;
            isLast = true;
        }
        
        // find directory entry in current directory
        FatDirectoryEntry entry;
        if (FatFindFile(disk, current, name, &entry))
        {
            FatClose(current);

            // check if directory
            if (!isLast && entry.Attributes & FatATTRIBUTE_DIRECTORY == 0)
            {
                Printk("FAT: %s not a directory\r\n", name);
                return NULL;
            }

            // open new directory entry
            current = FatOpenEntry(disk, &entry);
        }
        else
        {
            FatClose(current);

            Printk("FAT: %s not found\r\n", name);
            return NULL;
        }
    }

    return current;
}
