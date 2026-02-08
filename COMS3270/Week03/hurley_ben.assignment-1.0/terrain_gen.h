#ifndef TERRAIN_H
#define TERRAIN_H
#include "queue.h"


typedef struct gate {
    int gate_x;
    int gate_y;
} gate;

typedef struct map {
    gate g[4];

} map;

typedef struct world{
        map m[400][400];
}

int generate_start(terrain t_type);


#endif
