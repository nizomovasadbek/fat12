#ifndef _BOOT_H
#define _BOOT_H

#include <stdint.h>

typedef struct {
    uint8_t jumpInstruction[3];
    uint8_t oemIdentifier[8];
    uint16_t bytesPerSector;
    uint8_t sectorPerCluster;
    uint16_t reserved;
    uint8_t fatCount;
    uint16_t rootDirEntry;
    uint16_t totalSectors;
    uint8_t mediaDescriptorType;
    uint16_t sectorPerFat;
    uint16_t sectorPerTrack;
    uint16_t heads;
    uint32_t hiddenSectors;
    uint32_t largeSectorCount;

    uint8_t driveNumber;
    uint8_t flagsInNT;
    uint8_t signature;
    uint32_t volumeId;
    uint8_t volumeLabel[11];
    uint8_t identifierString[8];
} __attribute__((packed)) BootSector;

#endif