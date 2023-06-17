#include "interface.h"
#include "boot.h"
#include "entry.h"
#include <stdio.h>
#include <stdlib.h>

void readSector(FILE* disk, uint32_t lba, uint32_t count, void* out) {
    fseek(disk, lba * 512, SEEK_SET);
    fread(out, 512, count, disk);
}

bool readBootSector(BootSector* boot, FILE* disk) {
    return fread(boot, sizeof(BootSector), 1, disk);
}

bool entryRead(Entry* entry, FILE* disk, uint16_t skip) {
    fseek(disk, skip, SEEK_SET);
    return fread(entry, sizeof(Entry), 1, disk);
}

uint16_t getEntryFilesCount(FILE* disk, uint16_t start) {
    uint16_t count = 0;
    Entry* entry = (Entry*) malloc(sizeof(Entry));
    while(true) {
        entryRead(entry, disk, start);
        if(!entry->fileSize) break;
        count++;
        start += 32;
    }

    free(entry);
    return count;
}