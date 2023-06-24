#include "lib/interface.h"
#include "lib/boot.h"
#include "lib/entry.h"
#include "lib/fat12.h"
#include <stdio.h>
#include <stdlib.h>

BootSector* _boot;

void readSector(FILE* disk, uint32_t lba, uint32_t count, void* out) {
    if(lba)
        fseek(disk, lba * _boot->bytesPerSector, SEEK_SET);
    (void)fread(out, _boot->bytesPerSector, count, disk);
}

bool readBootSector(BootSector* boot, FILE* disk) {
    bool ok = fread(boot, sizeof(BootSector), 1, disk);
    _boot = boot;
    return ok;
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

bool readFile(FILE* disk, Entry* entry, void* out) {
    uint16_t fileStartSector = _boot->reserved;
    uint16_t chain = 0;
    uint16_t cluster = entry->startCluster;
    uint8_t* fat = (uint8_t*) malloc(_boot->fatCount * _boot->sectorPerFat * _boot->bytesPerSector);
    if(fat == NULL) return false;
    fileStartSector += _boot->fatCount * _boot->sectorPerFat;
    fileStartSector += (_boot->rootDirEntry * 32) / 512;
    const uint16_t FIRST_STATE_CLUSTER = fileStartSector;
    fileStartSector += entry->startCluster - 2;
    readSector(disk, _boot->reserved, _boot->sectorPerFat * _boot->fatCount, fat);

    do {
        readSector(disk, fileStartSector, 1, out);
        chain = decode(cluster, fat);
        if(chain <= 0x0FF8)
            out += _boot->bytesPerSector;
        fileStartSector = FIRST_STATE_CLUSTER;
        fileStartSector += chain - 2;
        cluster = chain;
    } while(chain <= 0x0FF8);

    free(fat);
    return true;
}