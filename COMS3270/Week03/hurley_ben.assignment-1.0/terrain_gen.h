#ifndef TERRAIN_H
#define TERRAIN_H
#include "queue.h"

typedef struct tile {
    int pos_x;
    int pos_y;
    int player;
    terrain type;
    int can_traverse;
} tile;

typedef struct gate {
    int gate_pos;
} gate;

typedef struct map {
    // gate g[4] goes in order of north, south, east, west
    gate g[4];
    
    tile t[21][80];

} map;

typedef struct world{
        map *m[401][401];
        int current_x;
        int current_y;
} world;

int generate_start(terrain t_type, map *m, terrain seeds[7], queue *q);

int generate_roads(map *m, world w, gate g[4]);

int generate_builds(map *m, int roll);

int map_print(map *m);


#endif
