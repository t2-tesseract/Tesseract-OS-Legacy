#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/TextMode/Printk.h>
#include <Include/Common/Common.h>
#include "Disk.h"

#define MEMORY_MIN          0x00000500
#define MEMORY_MAX          0x00080000

#define MEMORY_FAT_ADDR     ((void*)0x20000)
#define MEMORY_FAT_SIZE     0x00010000

typedef struct 
{
    uint8_t Name[11];
    uint8_t Attributes;
    uint8_t _Reserved;
    uint8_t CreatedTimeTenths;
    uint16_t CreatedTime;
    uint16_t CreatedDate;
    uint16_t AccessedDate;
    uint16_t FirstClusterHigh;
    uint16_t ModifiedTime;
    uint16_t ModifiedDate;
    uint16_t FirstClusterLow;
    uint32_t Size;
} __attribute__((packed)) FatDirectoryEntry;

typedef struct 
{
    int Handle;
    bool IsDirectory;
    uint32_t Position;
    uint32_t Size;
} FatFile;

enum FatAttributes
{
    FatATTRIBUTE_READ_ONLY         = 0x01,
    FatATTRIBUTE_HIDDEN            = 0x02,
    FatATTRIBUTE_SYSTEM            = 0x04,
    FatATTRIBUTE_VOLUME_ID         = 0x08,
    FatATTRIBUTE_DIRECTORY         = 0x10,
    FatATTRIBUTE_ARCHIVE           = 0x20,
    FatATTRIBUTE_LFN               = FatATTRIBUTE_READ_ONLY | FatATTRIBUTE_HIDDEN | FatATTRIBUTE_SYSTEM | FatATTRIBUTE_VOLUME_ID
};

bool FatInitialize(DISK* disk);
FatFile * FatOpen(DISK* disk, const char* path);
uint32_t FatRead(DISK* disk, FatFile* file, uint32_t byteCount, void* dataOut);
bool FatReadEntry(DISK* disk, FatFile* file, FatDirectoryEntry* dirEntry);
void FatClose(FatFile* file);
