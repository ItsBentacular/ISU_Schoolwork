#ifndef TERRAIN_H
#define TERRAIN_H
#include "queue.h"

typedef struct tile {
    int pos_x;
    int pos_y;
    int player;
    terrain type;
} tile;

typedef struct gate {
    int gate_pos;
} gate;

typedef struct map {
    gate g[4];
    tile t[21][80];

} map;

typedef struct world{
        map *m[401][401];
} world;

int generate_start(terrain t_type, map *m, terrain seeds[7], queue *q);

int generate_roads(map *m);

int generate_builds(map *m);

int map_print(map *m);


#endif
