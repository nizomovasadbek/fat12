#include <stdio.h>
#include <stdlib.h>
#include "boot.h"
#include "console.h"
#include "entry.h"
#include "interface.h"
#include "error.h"

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

    // uint8_t fileOrder = 1;
    // TODO: input file selection

    uint8_t* fat = (uint8_t*) malloc(boot->bytesPerSector);

    readSector(disk, 2, 1, fat);
    uint16_t temp = 0;
    uint16_t* format = (uint16_t*) fat;
    for(unsigned int i = 0; i < 20; i++) {
        temp = format[i];
        if(temp & 1) temp >>= 4;
        else temp &= 0x0FFF;
        printf("%03X ", temp);
    }
    printf("\n");

    free(boot);
    free(entries);
    free(fat);
    fclose(disk);
    return 0;
}