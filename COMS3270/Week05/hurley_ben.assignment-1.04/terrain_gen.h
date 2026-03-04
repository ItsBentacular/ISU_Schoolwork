#ifndef TERRAIN_H
#define TERRAIN_H
#include "heap.h"

typedef enum terrain {
    TERRAIN_BORDER = 127,
    TERRAIN_ROCK = '%',
    TERRAIN_WATER = '~',
    TERRAIN_GRASS = ':',
    TERRAIN_CLEAR = '.',
    TERRAIN_TREES = '^',
    TERRAIN_ROAD = '#',
    TERRAIN_GATE = 125,
    TERRAIN_EMPTY = ' ',
    TERRAIN_POKEC = 'C',
    TERRAIN_POKEM = 'M'
} terrain;

typedef enum character_type {
        PC = '@',
        HIKER = 'h',
        RIVAL = 'r',
        SWIMMER = 'q',
        PACER = 'p',
        WANDERER = 'w',
        SENTRY = 's',
        EXPLORER = 'e'
} character_type;

typedef struct character {
    int x;
    int y;
    character_type type;
    int next_turn;
    int sequence_num;
    int dir_x;
    int dir_y;
} character;

typedef struct tile {
    int pos_x;
    int pos_y;
    int player;
    terrain type;
    int traverse_cost;
} tile;

typedef struct gate {
    int gate_pos;
} gate;

typedef struct map {
    // gate g[4] goes in order of north, south, east, west
    gate g[4];
    
    tile t[21][80];
    int hiker_dist[21][80];
    int rival_dist[21][80];
    character* characters[21][80];

} map;

typedef struct world{
        map *m[401][401];
        int current_x;
        int current_y;
} world;

int generate_start(terrain t_type, map *m, terrain seeds[7], heap_t *h);

int generate_roads(map *m, world w, gate g[4]);

int generate_builds(map *m, int roll);

int map_print(map *m);

int move_cost(character_type c, terrain t);

void place_pc(map *m, heap_t*);

void place_npc(map *m, int num_trainers, heap_t *h);

void move_npc(character *c, map *m);

void dijkstra_path(map *m, character_type type);

int32_t character_cmp(const void *key, const void *with);


#endif
