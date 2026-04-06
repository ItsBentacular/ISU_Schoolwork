#ifndef POKE_H
#define POKE_H
#include "heap.h"

enum terrain {
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
};

enum character_type {
        PC = '@',
        HIKER = 'h',
        RIVAL = 'r',
        SWIMMER = 'q',
        PACER = 'p',
        WANDERER = 'w',
        SENTRY = 's',
        EXPLORER = 'e'
};

class character {
public:
    int x;
    int y;
    character_type type;
    int next_turn;
    int sequence_num;
    int dir_x;
    int dir_y;
    int isDefeated;
    char * charName; 

    character() {
        x = 0;
        y = 0;
        type = (character_type)NULL;
        next_turn = 0;
        sequence_num = 0;
        dir_x = 0;
        dir_y = 0;
        isDefeated = 0;
        charName = NULL;
    }
    character(int x1, int y1, character_type type1, int dir_x1, int dir_y1, int sequence_num1, int next_turn1) {
        x = x1;
        y = y1;
        type = type1;
        dir_x = dir_x1;
        dir_y = dir_y1;
        sequence_num = sequence_num1;
        next_turn = next_turn1;
        isDefeated = 0;
        charName = NULL;
    } 

    ~character() {}
};

class tile {
public:
    int pos_x;
    int pos_y;
    int player;
    terrain type;
    int traverse_cost;

    // Constructor
    tile() {
        pos_x = 0;
        pos_y = 0;
        player = 0;
        type = TERRAIN_EMPTY;
        traverse_cost = 0;
    }

    // Destructor
    ~tile() {}
};

class gate {
public:
    int gate_pos;

    int is_gate(int pos) {
        if(pos == gate_pos) {
            return 1;
        }
        else { return 0; }
    }
};

class map {
public:
    // gate g[4] goes in order of north, south, east, west
    gate g[4];
    tile t[21][80];

    int hiker_dist[21][80] = {};
    int rival_dist[21][80]= {};
    character* characters[21][80]= {};

    map() = default;

    ~map() = default;

};

class world{
public:
        map *m[401][401];
        int current_x;
        int current_y;
    world() {
        current_x = 200;
        current_y = 200;
    }
    ~world() {}
};

int generate_start(terrain t_type, map *m, terrain seeds[7], heap_t *h);

int generate_roads(map *m, world *w, gate g[4]);

int generate_builds(map *m, int roll);

int map_print(map *m);

int move_cost(character_type c, terrain t);

void place_pc(map *m, heap_t*);

void place_npc(map *m, int num_trainers, heap_t *h);

character* move_npc(character *c, map *m);

void generate_names(map *m);

void dijkstra_path(map *m, character_type type);

int32_t character_cmp(const void *key, const void *with);


#endif
