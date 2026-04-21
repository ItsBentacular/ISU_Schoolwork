#ifndef POKE_H
#define POKE_H
#include "heap.h"
#include <vector>

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

struct pokemon;
struct moves;
struct pokemonMoves;
struct pokemonStats;

class PokemonInstance {
public:
    const pokemon* base_species;
    int level;
    int ivs[6];           // [0]=HP, [1]=Atk, [2]=Def, [3]=Sp.Atk, [4]=Sp.Def, [5]=Speed
    int stats[6];         // [0]=HP, [1]=Atk, [2]=Def, [3]=Sp.Atk, [4]=Sp.Def, [5]=Speed
    int current_hp;
    std::vector<const moves*> known_moves;
    int gender;           // 0 for male, 1 for female
    bool is_shiny;

    PokemonInstance() : base_species(NULL), level(1), current_hp(1), gender(0), is_shiny(false) {
        for (int i = 0; i < 6; i++) {
            ivs[i] = 0;
            stats[i] = 0;
        }
    }
    ~PokemonInstance() {}
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
    int num_potions;
    int num_pokeballs;
    int num_revives;
    
    PokemonInstance* pokemon_party[6];
    int num_pokemon;

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
        num_pokemon = 0;
        num_potions = 6;
        num_pokeballs = 5;
        num_revives = 3;
        for (int i = 0; i < 6; i++) {
            pokemon_party[i] = NULL;
        }
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
        num_pokemon = 0;
        num_potions = 6;
        num_pokeballs = 5;
        num_revives = 3;
        for (int i = 0; i < 6; i++) {
            pokemon_party[i] = NULL;
        }
    } 

    ~character() {
        for (int i = 0; i < num_pokemon; i++) {
            if (pokemon_party[i] != NULL) {
                delete pokemon_party[i];
                pokemon_party[i] = NULL;
            }
        }
        // Note: keeping charName untouched here per original scope 
    }
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

void place_npc(map *m, int num_trainers, heap_t *h, int man_dis, 
               const std::vector<struct pokemon>& pokemon, 
               const std::vector<pokemonMoves>& poke_moves, 
               const std::vector<moves>& moves_list, 
               const std::vector<pokemonStats>& poke_stats);

character* move_npc(character *c, map *m);

void generate_names(map *m);

void dijkstra_path(map *m, character_type type);

int32_t character_cmp(const void *key, const void *with);

PokemonInstance* generate_pokemon(int manhattan_distance, 
                                  const std::vector<struct pokemon>& pokemon, 
                                  const std::vector<pokemonMoves>& poke_moves, 
                                  const std::vector<moves>& moves_list, 
                                  const std::vector<pokemonStats>& poke_stats);


#endif
