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
        skip += 32;
    }

    free(boot);
    free(entries);
    fclose(disk);
    return 0;
}