#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/Common/Common.h>
#include "Disk.h"

bool DiskInitialize(DISK* disk, uint8_t driveNumber)
{
    uint8_t driveType;
    uint16_t cylinders, sectors, heads;

    if (!DiskGetDriveParams(disk->id, &driveType, &cylinders, &sectors, &heads))
        return false;

    disk->id = driveNumber;
    disk->cylinders = cylinders;
    disk->heads = heads;
    disk->sectors = sectors;

    return true;
}

void DiskLBA2CHS(DISK* disk, uint32_t lba, uint16_t* cylinderOut, uint16_t* sectorOut, uint16_t* headOut)
{
    *sectorOut = lba % disk->sectors + 1;
    *cylinderOut = (lba / disk->sectors) / disk->heads;
    *headOut = (lba / disk->sectors) % disk->heads;
}

bool DiskReadSectors(DISK* disk, uint32_t lba, uint8_t sectors, void* dataOut)
{
    uint16_t cylinder, sector, head;

    DiskLBA2CHS(disk, lba, &cylinder, &sector, &head);

    for (int i = 0; i < 3; i++)
    {
        if (DiskRead(disk->id, cylinder, sector, head, sectors, dataOut))
            return true;

        DiskReset(disk->id);
    }

    return false;
}