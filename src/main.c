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
    printBootSector(boot);

    Entry* entry = (Entry*) malloc(sizeof(Entry));
    uint16_t entryStartSector = boot->reserved + boot->fatCount * boot->sectorPerFat;
    entryStartSector *= boot->bytesPerSector;
    if(!entryRead(entry, disk, entryStartSector)) {
        printf("Error reading entry\n");
        free(boot);
        free(entry);
        fclose(disk);
        return ENTRY_READ_ERROR;
    }

    printEntry(entry);

    free(boot);
    fclose(disk);
    return 0;
}