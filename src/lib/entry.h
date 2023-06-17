#ifndef _ENTRY_H
#define _ENTRY_H

#include <stdint.h>

typedef struct {
    uint8_t filename[8];
    uint8_t extension[3];
    uint8_t fileAttribute;
    uint8_t fileCase;
    uint8_t creationTimems;
    uint16_t creationTime;
    uint16_t creationDate;
    uint16_t lastAccessDate;
    uint16_t reserved;
    uint16_t lastModificationTime;
    uint16_t lastModificationDate;
    uint16_t startCluster;
    uint32_t fileSize;
} __attribute__((packed)) Entry;

#endif