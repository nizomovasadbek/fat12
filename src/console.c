#include "console.h"

void printEntry(Entry* entry) {
    printf("Filename: %s\n", entry->filename);
    printf("File attribute: %u\n", entry->fileAttribute);
    printf("File case: %u\n", entry->fileCase);
    printf("Creation time (ms): %u\n", entry->creationTimems);
    printf("Creation time: %u\n", entry->creationTime);
    printf("Creation date: %u\n", entry->creationDate);
    printf("Last access date: %u\n", entry->lastAccessDate);
    printf("Reserved: %u\n", entry->reserved);
    printf("Last modification time: %u\n", entry->lastModificationTime);
    printf("Last modification date: %u\n", entry->lastModificationDate);
    printf("Starting cluster: %u\n", entry->startCluster);
    printf("File size: %u\n", entry->fileSize);
}

void printBootSector(BootSector* boot) {
    printf("Jump instruction %X-%X-%X\n", boot->jumpInstruction[0], 
        boot->jumpInstruction[1], boot->jumpInstruction[2]);
    printf("OEM identifier: %s\n", boot->oemIdentifier);
    printf("Bytes per sector: %u\n", boot->bytesPerSector);
    printf("Sector per cluster: %u\n", boot->sectorPerCluster);
    printf("Reserved: %u\n", boot->reserved);
    printf("FAT count: %u\n", boot->fatCount);
    printf("Root directory entry count: %u\n", boot->rootDirEntry);
    printf("Total sectors: %u\n", boot->totalSectors);
    printf("Media descriptor type: %u\n", boot->mediaDescriptorType);
    printf("Sectors per fat: %u\n", boot->sectorPerFat);
    printf("Sectors per track: %u\n", boot->sectorPerTrack);
    printf("Heads: %u\n", boot->heads);
    printf("Hidden sectors: %u\n", boot->hiddenSectors);
    printf("Large sector count: %u\n", boot->largeSectorCount);
    printf("Drive number: %u\n", boot->driveNumber);
    printf("Flag in NT: %u\n", boot->flagsInNT);
    printf("Signature: 0x%X\n", boot->signature);
    printf("Volume ID: %u\n", boot->volumeId);
    printf("Volume label: %s\n", boot->volumeLabel);
    printf("System identifier: %s\n", boot->identifierString);
}