#include "lib/fat12.h"

uint16_t decode(uint16_t cluster, uint8_t* fatSector) {
    uint16_t clusterResult = cluster;
    uint32_t fatIndex = clusterResult * 3 / 2;

    if(cluster & 1) {
        clusterResult = *((uint16_t*)(fatSector + fatIndex)) >> 4;
    } else {
        clusterResult = *((uint16_t*)(fatSector + fatIndex)) & 0x0FFF;
    }

    return clusterResult;
}