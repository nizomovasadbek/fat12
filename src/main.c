#include <stdio.h>
#include <stdlib.h>
#include "boot.h"
#include "console.h"
#include "interface.h"
#include "error.h"

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Error syntax\n");
        printf("Use: <main> <disk image>\n");
        return -1;
    }

    FILE* disk = fopen(argv[1], "rb");
    BootSector* boot = (BootSector*) malloc(sizeof(BootSector));
    if(!readBootSector(boot, disk)) {
        printf("Couldn't read boot sector\n");
        return 1;
    }
    printBootSector(boot);

    uint16_t entryStartSector = boot->reserved + boot->fatCount * boot->sectorPerFat;
    entryStartSector *= boot->bytesPerSector;

    free(boot);
    fclose(disk);
    return 0;
}