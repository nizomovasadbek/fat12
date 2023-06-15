#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <stdio.h>
#include "boot.h"
#include "entry.h"

#define enter() printf("-----------------------Enter-----------------------\n")
#define leave() printf("-----------------------Leave------------------------\n")

void printBootSector(BootSector*);
void printEntry(Entry*);

#endif