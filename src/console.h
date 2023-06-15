#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <stdio.h>
#include "boot.h"
#include "entry.h"

void printBootSector(BootSector*);
void printEntry(Entry*);

#endif