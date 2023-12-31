#include <stdio.h>
#include <stdlib.h>
#include "lib/boot.h"
#include "lib/entry.h"
#include "lib/interface.h"
#include "lib/error.h"

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
        if(!entryRead(&entries[i], disk, skip)) {
            printf("Warning couldn't read entry %s\n", entries[i].filename);
            continue;
        }
        printf("%u) Filename: %s\t%u B\n", (i+1), entries[i].filename, entries[i].fileSize);
        skip += 32;
    }

    uint32_t fileOrder = 0;
    char buffer[10];
    printf("Enter file number: ");
    scanf("%9s", buffer);
    sscanf(buffer, "%d", &fileOrder);

    if(fileOrder > entryFilesCount || !fileOrder) {
        free(boot);
        free(entries);
        printf("File with that order doesn't exist!\n");
        return SUCH_FILE_DOESNT_EXIST;
    }
    Entry* entry = &entries[fileOrder - 1];

    uint32_t capacity = entry->fileSize / boot->bytesPerSector;
    capacity++;
    capacity *= boot->bytesPerSector;
    capacity++;
    uint8_t* out = (uint8_t*) malloc(capacity);
    (void)readFile(disk, entry, out);
    out[capacity-1] = 0;

    printf("\n%s", out);

    printf("\n");

    free(out);
    free(boot);
    free(entries);
    fclose(disk);
    return 0;
}
