#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "boot.h"
#include "entry.h"
#include <stdio.h>
#include <stdbool.h>

bool readBootSector(BootSector*, FILE*);
bool entryRead(Entry*, FILE*, uint16_t);
uint16_t getEntryFilesCount(FILE*, uint16_t);
void readSector(FILE*, uint32_t, uint32_t, void*);

#endif