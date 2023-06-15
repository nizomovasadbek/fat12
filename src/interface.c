#include "interface.h"
#include "boot.h"
#include "entry.h"
#include <stdio.h>

bool readBootSector(BootSector* boot, FILE* disk) {
    return fread(boot, sizeof(BootSector), 1, disk);
}

bool entryRead(Entry* entry, FILE* disk, uint16_t skip) {
    fseek(disk, skip, SEEK_SET);
    return fread(entry, sizeof(Entry), 1, disk);
}