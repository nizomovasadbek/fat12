#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "boot.h"
#include "entry.h"
#include <stdio.h>
#include <stdbool.h>

bool readBootSector(BootSector*, FILE*);
bool entryRead(Entry*, FILE*, uint16_t);

#endif