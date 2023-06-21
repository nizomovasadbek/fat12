#include <stdio.h>
#include <stdlib.h>
#include "lib/boot.h"
#include "lib/console.h"
#include "lib/entry.h"
#include "lib/fat12.h"
#include "lib/interface.h"
#include "lib/error.h"

#define FAT_OFFSET 2

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Error syntax\n");
        printf("Use: <main> <disk image>\n");
        return SYNTAX_ERROR;
    }

    FILE* disk = fopen(argv[1], "rb");
    if(!disk) {
        printf("Disk reading error: %s\n", argv[1]);
        return DISK_OPEN_ERROR;
    }
    BootSector* boot = (BootSector*) malloc(sizeof(BootSector));
    if(!readBootSector(boot, disk)) {
        printf("Couldn't read boot sector\n");
        free(boot);
        fclose(disk);
        return BOOTSECTOR_READ_ERROR;
    }

    uint16_t entryStartSector = boot->reserved + boot->fatCount * boot->sectorPerFat;
    entryStartSector *= boot->bytesPerSector;
    uint16_t entryFilesCount = getEntryFilesCount(disk, entryStartSector);
    printf("There are %u files\n", entryFilesCount);

    Entry* entries = (Entry*) calloc(entryFilesCount, sizeof(Entry));
    uint16_t skip = entryStartSector;
    for(unsigned int i = 0; i < entryFilesCount; i++) {
        entryRead(&entries[i], disk, skip);
        printf("%u) Filename: %s\t%u Kb\n", (i+1), entries[i].filename, entries[i].fileSize);
        skip += 32;
    }

    uint32_t fileOrder = 2;
    //scanf("%u", &fileOrder);
    Entry* entry = &entries[fileOrder - 1];

    uint8_t* fat = (uint8_t*) malloc(boot->bytesPerSector);
    uint16_t currentCluster = entry->startCluster;
    readSector(disk, boot->reserved, 1, fat);
    
    for(unsigned int i = 0; i < 12; i++) {
        printf("%02X ", fat[i]);
    }

    currentCluster = decode(FAT_OFFSET + (entry->startCluster - 2), fat);
    printf("\nCurrent cluster: %u-%03X\n", currentCluster, currentCluster);
    printEntry(entry);

    printf("\n");

    free(boot);
    free(entries);
    free(fat);
    fclose(disk);
    return 0;
}