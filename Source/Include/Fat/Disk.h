#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t id;
    uint16_t cylinders;
    uint16_t sectors;
    uint16_t heads;
} DISK;

#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

bool DiskInitialize(DISK* disk, uint8_t driveNumber);
bool DiskReadSectors(DISK* disk, uint32_t lba, uint8_t sectors, void* lowerDataOut);
