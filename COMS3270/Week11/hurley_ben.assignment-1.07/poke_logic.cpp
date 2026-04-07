#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "heap.h"
#include "poke_logic.hpp"
#include <limits.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>


#define ROW 21
#define COLUMN 80

typedef struct gen_point {
    int x;
    int y;
    terrain type;
    int sequence; // Used to maintain FIFO order in the heap (my old queue code was lacking in comparison to the heap code).
} gen_point_t;

static int32_t gen_point_compare(const void *key, const void *with) {
    return ((const gen_point_t *)key)->sequence - ((const gen_point_t *)with)->sequence;
}

int32_t character_cmp(const void *key, const void *with) {
    const character *c1 = (const character *)key;
    const character *c2 = (const character *)with;
    if (c1->next_turn != c2->next_turn) {
        return c1->next_turn - c2->next_turn;
    }
    return c1->sequence_num - c2->sequence_num;
}

// this function allows the borders to be defined as different values while generating as terrain.
int generate_start(terrain border, map *m, terrain seeds[7], heap_t *h){
        int sequence_counter = 0;

        for(int i = 0; i < ROW; i++) {
            for(int j = 0; j < COLUMN; j++) {
                if((i == 0 || i == ROW -1) || (j == 0 || j == COLUMN -1)) {
                    m->t[i][j].type = border;
                } else {
                m->t[i][j].type = TERRAIN_EMPTY;
                }
            }
        }

    //queues terrain types at random positions, then places the terrain onto the map.
    heap_init(h, gen_point_compare, free);

    for(int i = 0; i < 7; i++) {
        int ry = rand() % 19 + 1;
        int rx = rand() % 79 + 1;
        
        gen_point_t *pt = new gen_point_t;
        pt->x = rx;
        pt->y = ry;
        pt->sequence = sequence_counter++;

        // not sure how efficient this switch case is, but it does a pretty good job at allowing the queue to be semi-random by randomizing the ry and rx values within the loop while keeping the terrain types the same.
        switch(i){
            case 0: pt->type = seeds[0]; break;
            case 1: pt->type = seeds[1]; break;
            case 2: pt->type = seeds[2]; break;
            case 3: pt->type = seeds[3]; break;
            case 4: pt->type = seeds[4]; break;
            case 5: pt->type = seeds[5]; break;
            case 6: pt->type = seeds[6]; break;
        }

        heap_insert(h, pt);
        m->t[ry][rx].type = pt->type;
    }

    // generates more terrain around the 'seeds' by checking if the terrains surrounding area is in bounds, then puts the same type of terrain around it, and requeues the new terrain onto the queue and repeats until map is full.
    while(h->size > 0) {
        int from_x, from_y;
        terrain from_type;
        int neighbor_x[] = {0,0,1,-1};
        int neighbor_y[] = {-1,1,0,0};

        // Extract from heap
        gen_point_t *p = (gen_point_t *)heap_remove_min(h);
        from_x = p->x;
        from_y = p->y;
        from_type = p->type;
        free(p); // We must free the wrapper, be free!

        for(int i = 0; i < 4; i++) {
            int to_x = from_x + neighbor_x[i];
            int to_y = from_y + neighbor_y[i];
            if((to_x < COLUMN - 1 && to_x > 0) && (to_y < ROW - 1 && to_y > 0) && m->t[to_y][to_x].type == TERRAIN_EMPTY) {

                m->t[to_y][to_x].type = from_type;
                
                gen_point_t *next_pt = new gen_point_t();
                next_pt->x = to_x;
                next_pt->y = to_y;
                next_pt->type = from_type;
                next_pt->sequence = sequence_counter++;
                heap_insert(h, next_pt);
            }
        }
    }
    heap_delete(h);

    return 0;
}

int generate_roads(map *m, world *w, gate g[4]) {
    // finds a random point NOT in the corners for the gates, then draws lines out of # to a certain x or y coord, then draws another line between the ends of the two lines
    // addendum: now also checks if we are in world bounds, then if there is already a map generated in a spot, make the opposite road connect to the new maps road,
    // ex: north connects to new maps south, west connects to east, else generate random roads.
    int n_gate,s_gate,e_gate,w_gate;
    // gate g[4] goes in order of north, south, east, west

    //north map
    if(w->current_y < 400 && w->m[w->current_x][w->current_y + 1] != NULL) {
        n_gate = w->m[w->current_x][w->current_y + 1]->g[1].gate_pos;
    } else {
        n_gate = rand() % (COLUMN -2) + 1;
    }
    //south map
    if(w->current_y > 0 && w->m[w->current_x][w->current_y - 1] != NULL) {
        s_gate = w->m[w->current_x][w->current_y - 1]->g[0].gate_pos;
    } else {
        s_gate = rand() % (COLUMN -2) + 1;
    }
    //east map
    if(w->current_x < 400 && w->m[w->current_x + 1][w->current_y] != NULL) {
        e_gate = w->m[w->current_x + 1][w->current_y]->g[3].gate_pos;
    } else {
        e_gate = rand() % (ROW - 2) + 1;
    }
    //west map
    if(w->current_x > 0 && w->m[w->current_x - 1][w->current_y] != NULL) {
        w_gate = w->m[w->current_x - 1][w->current_y]->g[2].gate_pos;
    } else {
        w_gate = rand() % (ROW - 2) + 1;
    }

    int road_y = (rand() % 10) + 5;
    int road_x = (rand() % 40) + 20;

    // generates the east and west roads.
    for(int i = (w->current_x == 0) ? 1 : 0; i <= road_x; i++) {
        m->t[w_gate][i].type = TERRAIN_ROAD;
    }
    for(int i = (w->current_x == 400) ? COLUMN - 2 : COLUMN - 1; i >= road_x; i--) {
        m->t[e_gate][i].type = TERRAIN_ROAD;
    }

    // connects the east and west roads.
    int start_y, end_y;
    if(w_gate < e_gate) {
        start_y = w_gate;
        end_y = e_gate;
    } else {
        start_y = e_gate;
        end_y = w_gate;
    }
    for(int i = start_y; i <= end_y; i++) {
        m->t[i][road_x].type = TERRAIN_ROAD;
    }

    // generates the north and south roads.
    for(int i = (w->current_y == 400) ? 1 : 0; i <= road_y; i++) {
        m->t[i][n_gate].type = TERRAIN_ROAD;
    }
    for(int i = (w->current_y == 0) ? ROW - 2 : ROW - 1; i >= road_y; i--) {
        m->t[i][s_gate].type = TERRAIN_ROAD;
    }

    //connects the north and south roads.
    int start_x, end_x;
    if(s_gate < n_gate) {
        start_x = s_gate;
        end_x = n_gate;
    } else {
        start_x = n_gate;
        end_x = s_gate;
    }
    for(int i = start_x; i <= end_x; i++) {
        m->t[road_y][i].type = TERRAIN_ROAD;
    }

    g[0].gate_pos = n_gate;
    g[1].gate_pos = s_gate;
    g[2].gate_pos = e_gate;
    g[3].gate_pos = w_gate;

    if (w->current_y < 400) m->t[0][n_gate].type = TERRAIN_GATE;
    if (w->current_y > 0) m->t[ROW - 1][s_gate].type = TERRAIN_GATE;
    if (w->current_x < 400) m->t[e_gate][COLUMN - 1].type = TERRAIN_GATE;
    if (w->current_x > 0) m->t[w_gate][0].type = TERRAIN_GATE;

    return 0;
}

int generate_builds(map *m, int man_dis) {
    int chance;
    if (man_dis == 0) {
        chance = 100;
    } else {
        chance = 85 - ((3 * man_dis) / 8);
        if (chance < 10) chance = 10;
    }

    // generates the pokecenters (C) in a open area on the map that is attached to a road.
    int space_x, space_y;
    int building_placed = 0;
    if((rand() % 100) < chance) {
        while(building_placed == 0) {
        space_x = rand() % (COLUMN - 2) + 1;
        space_y = rand() % (ROW - 2) + 1;

        if(m->t[space_y][space_x].type == TERRAIN_ROAD && space_y > 3 && space_y < ROW - 3 && space_x > 3 && space_x < COLUMN - 3) {
                if (m->t[space_y-1][space_x].type != TERRAIN_ROAD && m->t[space_y-2][space_x].type != TERRAIN_ROAD && m->t[space_y-1][space_x+1].type != TERRAIN_ROAD && m->t[space_y-2][space_x+1].type != TERRAIN_ROAD) {

                    m->t[space_y-1][space_x].type   = (terrain)'C';
                    m->t[space_y-2][space_x].type   = (terrain)'C';
                    m->t[space_y-1][space_x+1].type = (terrain)'C';
                    m->t[space_y-2][space_x+1].type = (terrain)'C';
                    building_placed = 1;
            }
        }
        }
    }

    // generates the pokemarts (M) on the opposite side of the pokecenters, same rules follow.
    building_placed = 0;
    if((rand() % 100) < chance) {
        while(building_placed == 0) {
            space_x = rand() % (COLUMN - 2) + 1;
            space_y = rand() % (ROW - 2) + 1;

            if(m->t[space_y][space_x].type == TERRAIN_ROAD && space_y > 3 && space_y < ROW - 3 && space_x > 3 && space_x < COLUMN - 3) {

                if (m->t[space_y+1][space_x].type != TERRAIN_ROAD && m->t[space_y+1][space_x].type != 'C' && m->t[space_y+2][space_x].type != TERRAIN_ROAD && m->t[space_y+2][space_x].type != 'C' &&
                    m->t[space_y+1][space_x+1].type != TERRAIN_ROAD && m->t[space_y+1][space_x+1].type != 'C' &&
                    m->t[space_y+2][space_x+1].type != TERRAIN_ROAD && m->t[space_y+2][space_x+1].type != 'C') {
                    
                    m->t[space_y+1][space_x].type   = (terrain)'M';
                    m->t[space_y+2][space_x].type   = (terrain)'M';
                    m->t[space_y+1][space_x+1].type = (terrain)'M';
                    m->t[space_y+2][space_x+1].type = (terrain)'M';
                    building_placed = 1;
                }
            }
        }
    }

    return 0;
}

// prints final map of terrain.
int map_print(map *m){
    static int initialMap = 0;

    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COLUMN; j++) {
            // First, check if a character exists at this spot.
            if (m->characters[i][j] != NULL) {
                // If one does, print its character type ('@', 'h', 'r', etc.)
                mvaddch(i + 1, j , m->characters[i][j]->type);
            }
            // If no character, then print the terrain like before.
            else if (m->t[i][j].type == TERRAIN_BORDER) {
                attron(COLOR_PAIR(4));
                mvaddch(i + 1, j, '%');
                attroff(COLOR_PAIR(4));
            } else if (m->t[i][j].type == TERRAIN_GATE) {
                attron(COLOR_PAIR(3));
                mvaddch(i + 1, j, '#');
                attroff(COLOR_PAIR(3));
            } else {
                if(m->t[i][j].type == TERRAIN_WATER) {
                    attron(COLOR_PAIR(1));
                    mvaddch(i + 1, j, m->t[i][j].type);
                    attroff(COLOR_PAIR(1));
                }
                if(m->t[i][j].type == TERRAIN_GRASS || m->t[i][j].type == TERRAIN_CLEAR) {
                    attron(COLOR_PAIR(2));
                    mvaddch(i + 1, j, m->t[i][j].type);
                    attroff(COLOR_PAIR(2));
                }
                if(m->t[i][j].type == TERRAIN_ROCK) {
                    attron(COLOR_PAIR(4));
                    mvaddch(i + 1, j, m->t[i][j].type);
                    attroff(COLOR_PAIR(4));
                }
                if(m->t[i][j].type == TERRAIN_ROAD) {
                    attron(COLOR_PAIR(3));
                    mvaddch(i + 1, j, m->t[i][j].type);
                    attroff(COLOR_PAIR(3));
                }
                if(m->t[i][j].type == TERRAIN_TREES) {
                    attron(COLOR_PAIR(5));
                    mvaddch(i + 1, j, m->t[i][j].type);
                    attroff(COLOR_PAIR(5));
                }
                if(m->t[i][j].type == TERRAIN_POKEM || m->t[i][j].type == TERRAIN_POKEC) {
                    attron(COLOR_PAIR(6));
                    mvaddch(i + 1, j, m->t[i][j].type);
                    attroff(COLOR_PAIR(6));
                }
            }
            if(!initialMap) {
            usleep(300);
            refresh();
            }
        }
    }
    initialMap = 1;
    refresh();
    return 0;
}


//calculates move costs
int move_cost(character_type c, terrain t) {
    switch (c) {
        case PC:
            switch (t) {
                case TERRAIN_ROAD: return 10;
                // changing this GATE to intmax just for this assignment because it's causing seg faults, DONT FORGET TO CHANGE THIS BACK BEN
                case TERRAIN_GATE: return INT_MAX;
                case TERRAIN_GRASS: return 20;
                case TERRAIN_CLEAR: return 10;
                case TERRAIN_POKEM: return 10;
                case TERRAIN_POKEC: return 10;
                case TERRAIN_ROCK: return INT_MAX;
                case TERRAIN_TREES: return INT_MAX;
                case TERRAIN_WATER: return INT_MAX;
                case TERRAIN_BORDER: return INT_MAX;
                default: return INT_MAX;
            }
        case HIKER:
            switch (t) {
                case TERRAIN_ROAD: return 10;
                case TERRAIN_GRASS: return 15;
                case TERRAIN_CLEAR: return 10;
                case TERRAIN_ROCK: return 15;
                case TERRAIN_TREES: return 15;
                case TERRAIN_POKEM: return 50;
                case TERRAIN_POKEC: return 50;
                case TERRAIN_WATER: return INT_MAX;
                case TERRAIN_GATE: return INT_MAX;
                case TERRAIN_BORDER: return INT_MAX;
                default: return INT_MAX;
            }
        case RIVAL:
            switch (t) {
                case TERRAIN_ROAD: return 10;
                case TERRAIN_GRASS: return 20;
                case TERRAIN_CLEAR: return 10;
                case TERRAIN_POKEM: return 50;
                case TERRAIN_POKEC: return 50;
                case TERRAIN_ROCK: return INT_MAX;
                case TERRAIN_TREES: return INT_MAX;
                case TERRAIN_WATER: return INT_MAX;
                case TERRAIN_GATE: return INT_MAX;
                case TERRAIN_BORDER: return INT_MAX;
                default: return INT_MAX;
            }
        case SWIMMER:
            switch (t) {
                case TERRAIN_ROAD: return INT_MAX;
                case TERRAIN_GRASS: return INT_MAX;
                case TERRAIN_CLEAR: return INT_MAX;
                case TERRAIN_POKEM: return INT_MAX;
                case TERRAIN_POKEC: return INT_MAX;
                case TERRAIN_ROCK: return INT_MAX;
                case TERRAIN_TREES: return INT_MAX;
                case TERRAIN_WATER: return 7;
                case TERRAIN_GATE: return INT_MAX;
                case TERRAIN_BORDER: return INT_MAX;
                default: return INT_MAX;
            }
        case PACER:
        case WANDERER:
        case EXPLORER:
        case SENTRY:
            switch (t) {
                case TERRAIN_ROAD: return 10;
                case TERRAIN_GRASS: return 20;
                case TERRAIN_CLEAR: return 10;
                case TERRAIN_POKEM: return 50;
                case TERRAIN_POKEC: return 50;
                case TERRAIN_ROCK: return INT_MAX;
                case TERRAIN_TREES: return INT_MAX;
                case TERRAIN_WATER: return INT_MAX;
                case TERRAIN_GATE: return INT_MAX;
                case TERRAIN_BORDER: return INT_MAX;
                default: return INT_MAX;
            }
        default:
            return INT_MAX;
    }
}

// heap_t is a pointer so it links to the heap outside of the loop
void place_pc(map *m, heap_t *h) {
    int pcPlaced = 0;
    static int sequence_num = 0;

    while (!pcPlaced) {
        int x = rand() % (COLUMN - 2) + 1;
        int y = rand() % (ROW - 2) + 1;

        if (m->t[y][x].type == TERRAIN_ROAD && m->t[y][x].type != TERRAIN_GATE && m->characters[y][x] == NULL) {
            character *player_char = new character();
            if(player_char == NULL) {
                printf("failed to place character, reccommend restart");
                return;
            }
            m->characters[y][x] = player_char;
            m->characters[y][x]->type = PC;
            m->characters[y][x]->x = x;
            m->characters[y][x]->y = y;
            m->characters[y][x]->next_turn = 0;
            m->characters[y][x]->sequence_num = sequence_num++;
            m->characters[y][x]->dir_x = 0;
            m->characters[y][x]->dir_y = 0;
            pcPlaced = 1;
            heap_insert(h, m->characters[y][x]); 
        }
    }
}

// if num_trainers is set to -1, we randomize the number of trainers in a map, else follow what user sets.
void place_npc(map *m, int num_trainers, heap_t *h) {
    int npcPlaced = 0;
    int randNPC = 0;
    static int sequence_num = 1000; // Start higher than PC to avoid collisions at turn 0

    while(npcPlaced < num_trainers) {
        int x = rand() % (COLUMN - 2) + 1;
        int y = rand() % (ROW - 2) + 1;
        // 7 is number of NPC types (idk if thats a "magic number")
        randNPC = rand() % 7;
        //always one hiker, one rival
        if(npcPlaced == 0) {
            randNPC = 0;
        }
        if(npcPlaced == 1) {
            randNPC = 1;
        }
        switch(randNPC) {
            case 0:
            //hiker
                if (m->t[y][x].type != TERRAIN_BORDER && m->t[y][x].type != TERRAIN_GATE && m->t[y][x].type != TERRAIN_WATER && m->characters[y][x] == NULL) {
                    character *new_hiker = new character(x,y,HIKER,0,0,sequence_num++, 0);
                    if(new_hiker == NULL){
                        printf("character failed to spawn: Hiker");
                        break;
                    }

                    m->characters[y][x] = new_hiker;
                    npcPlaced++;
                    heap_insert(h, m->characters[y][x]); 
                }
            break;
            case 1:
            //rival
                if (m->t[y][x].type != TERRAIN_BORDER && m->t[y][x].type != TERRAIN_GATE && m->t[y][x].type != TERRAIN_WATER && m->t[y][x].type != TERRAIN_ROCK && m->t[y][x].type != TERRAIN_TREES && m->characters[y][x] == NULL) {
                    character *new_rival = new character(x,y,RIVAL,0,0,sequence_num++, 0);
                    if(new_rival == NULL){
                        printf("character failed to spawn: Rival");
                        break;
                    }

                    m->characters[y][x] = new_rival;
                    npcPlaced++;
                    heap_insert(h, m->characters[y][x]); 
                }
            break;
            case 2:
            // swimmer
                if (m->t[y][x].type == TERRAIN_WATER && m->characters[y][x] == NULL) {
                    character *new_swimmer = new character(x,y,SWIMMER,0,0,sequence_num++, 0);
                    if(new_swimmer == NULL){
                        printf("character failed to spawn: Swimmer");
                        break;
                    }

                    m->characters[y][x] = new_swimmer;
                    npcPlaced++;
                    heap_insert(h, m->characters[y][x]); 
                }
            break;
            case 3:
            // pacer
                if (m->t[y][x].type != TERRAIN_BORDER && m->t[y][x].type != TERRAIN_GATE && m->t[y][x].type != TERRAIN_WATER && m->t[y][x].type != TERRAIN_ROCK && m->t[y][x].type != TERRAIN_TREES && m->characters[y][x] == NULL) {
                    character *new_pacer = new character(x,y,PACER,0,0,sequence_num++, 0);
                    if(new_pacer == NULL){
                        printf("character failed to spawn: Pacer");
                        break;
                    }

                    m->characters[y][x] = new_pacer;
                    npcPlaced++;
                    heap_insert(h, m->characters[y][x]); 
                }
            break;
            case 4:
            // wanderer
                if (m->t[y][x].type != TERRAIN_BORDER && m->t[y][x].type != TERRAIN_GATE && m->t[y][x].type != TERRAIN_WATER && m->t[y][x].type != TERRAIN_ROCK && m->t[y][x].type != TERRAIN_TREES && m->characters[y][x] == NULL) {
                    character *new_wanderer = new character(x,y,WANDERER,0,0,sequence_num++, 0);
                    if(new_wanderer == NULL){
                        printf("character failed to spawn: Wanderer");
                        break;
                    }

                    m->characters[y][x] = new_wanderer;
                    npcPlaced++;
                    heap_insert(h, m->characters[y][x]); 
                }
            break;
            case 5:
            // sentry
                if (m->t[y][x].type != TERRAIN_BORDER && m->t[y][x].type != TERRAIN_GATE && m->t[y][x].type != TERRAIN_WATER && m->t[y][x].type != TERRAIN_ROCK && m->t[y][x].type != TERRAIN_TREES && m->characters[y][x] == NULL && m->t[y][x].type != TERRAIN_POKEC && m->t[y][x].type != TERRAIN_POKEM) {
                    character *new_sentry = new character(x,y,SENTRY,0,0,sequence_num++, 0);
                    if(new_sentry == NULL){
                        printf("character failed to spawn: Sentry");
                        break;
                    }

                    m->characters[y][x] = new_sentry;
                    npcPlaced++;
                    heap_insert(h, m->characters[y][x]); 
                }
            break;
            case 6:
            // explorer
                if (m->t[y][x].type != TERRAIN_BORDER && m->t[y][x].type != TERRAIN_GATE && m->t[y][x].type != TERRAIN_WATER && m->t[y][x].type != TERRAIN_ROCK && m->t[y][x].type != TERRAIN_TREES && m->characters[y][x] == NULL) {
                    character *new_explorer = new character(x,y,EXPLORER,0,0,sequence_num++, 0);
                    if(new_explorer == NULL){
                        printf("character failed to spawn: Explorer");
                        break;
                    }

                    m->characters[y][x] = new_explorer;
                    npcPlaced++;
                    heap_insert(h, m->characters[y][x]); 
                }
            break;
        }  
    }
}

void generate_names(map *m) {
    int randName = 0;
    const char * prefix;
    const char *names[] = {
        "David", "Sarah", "Peter", "Tequila Sunset", "Raphael Ambrosius Costeau",
        "Felix", "Red", "Char Mustard", "Dave", "Major Major Major",
        "Liz Anya", "Bud Light", "Carl", "Brock", "Guybrush Threepwood",
        "McLovin", "Chubbs Peterson", "John", "Jane", "Mike",
        "Emily", "Chris", "Jessica", "Matt", "Ashley",
        "Brian", "Amanda", "Luke", "Laura", "Kevin",
        "Megan", "Alex"
    };
    int num_names = sizeof(names) / sizeof(names[0]);

    for(int i = 0; i < 21; i++) {
        for (int j = 0; j < 80; j++) {
            if(m->characters[i][j] != NULL && m->characters[i][j]->type != PC) {
                randName = rand() % num_names;
                switch(m->characters[i][j]->type) {
                    case 'h':
                    prefix = "Hiker";
                    break;
                    case 'r': 
                    prefix = "Rival";
                    break;
                    case 'q': 
                    prefix = "Swimmer";
                    break;
                    case 'p': 
                    prefix = "Pacer";
                    break;
                    case 'w': 
                    prefix = "Wanderer";
                    break;
                    case 's': 
                    prefix = "Sentry";
                    break;
                    case 'e': 
                    prefix = "Explorer";
                    break;
                    default:
                    prefix = "Trainer";
                    break;
                }
                const char *name = names[randName];
                m->characters[i][j]->charName = new char[strlen(prefix) + strlen(name) + 2];
                sprintf(m->characters[i][j]->charName, "%s %s", prefix, name);
            }
        }
    } 
}

character* move_npc(character *c, map *m) {
    // don't do any calculations if the character is defeated.
    if (c->isDefeated) {
        c->next_turn += move_cost(c->type, m->t[c->y][c->x].type);
        return NULL; 
    }
    // movement arrays for the npcs that actually do pathfinding starts at right side (going to the right), goes counterclockwise in available movement options.
    int npcMoveX[] = {1,1,0,-1,-1,-1,0,1};
    int npcMoveY[] = {0,1,1,1,0,-1,-1,-1};
    int new_x = c->x;
    int new_y = c->y;
    int min_dist = INT_MAX;
    int cost = INT_MAX;
    character *battled = NULL;

    // Lazy initialization for directional NPCs
    if ((c->type == PACER || c->type == WANDERER || c->type == EXPLORER || c->type == SWIMMER) && 
        c->dir_x == 0 && c->dir_y == 0) {
        int dir_idx = rand() % 8;
        c->dir_x = npcMoveX[dir_idx];
        c->dir_y = npcMoveY[dir_idx];
    }
    
    switch(c->type) {
        case HIKER:
        case RIVAL:
            // look for the neighbor with the lowest distance to PC
            for(int i = 0; i < 8; i++) {
                int tx = c->x + npcMoveX[i];
                int ty = c->y + npcMoveY[i];
                
                if (tx > 0 && tx < COLUMN - 1 && ty > 0 && ty < ROW - 1) {
                    int dist = (c->type == HIKER) ? m->hiker_dist[ty][tx] : m->rival_dist[ty][tx];
                    
                    if (m->characters[ty][tx] != NULL && m->characters[ty][tx]->type == PC) {
                        battled = c;
                        new_x = c->x;
                        new_y = c->y;
                        break;
                    }

                    // Check if spot is better, not occupied, and not infinite cost
                    if (dist < min_dist && m->characters[ty][tx] == NULL && dist != INT_MAX) {
                        min_dist = dist;
                        new_x = tx;
                        new_y = ty;
                    }
                }
            }
            
        break; 
        case PACER:
            new_x = c->x + c->dir_x;
            new_y = c->y + c->dir_y;
            
            cost = move_cost(c->type, m->t[new_y][new_x].type);
            
            if (m->characters[new_y][new_x] != NULL && m->characters[new_y][new_x]->type == PC) {
                battled = c;
                new_x = c->x;
                new_y = c->y;
            } else if (cost == INT_MAX || m->characters[new_y][new_x] != NULL) {
                c->dir_x *= -1;
                c->dir_y *= -1;
                new_x = c->x; // Don't move this turn
                new_y = c->y;
            }
        break; 
        case WANDERER:
            new_x = c->x + c->dir_x;
            new_y = c->y + c->dir_y;
            
            cost = move_cost(c->type, m->t[new_y][new_x].type);
            
            if (m->characters[new_y][new_x] != NULL && m->characters[new_y][new_x]->type == PC) {
                battled = c;
                new_x = c->x;
                new_y = c->y;
            } else if (cost == INT_MAX || m->characters[new_y][new_x] != NULL || 
                m->t[new_y][new_x].type != m->t[c->y][c->x].type) {
                
                int dir_idx = rand() % 8;
                c->dir_x = npcMoveX[dir_idx];
                c->dir_y = npcMoveY[dir_idx];
                new_x = c->x; // Don't move this turn
                new_y = c->y;
            }
        break; 
        case SENTRY:
        // does nothing but watch the world go by...
        break; 
        case EXPLORER:
            new_x = c->x + c->dir_x;
            new_y = c->y + c->dir_y;
            
            cost = move_cost(c->type, m->t[new_y][new_x].type);
            
            if (m->characters[new_y][new_x] != NULL && m->characters[new_y][new_x]->type == PC) {
                battled = c;
                new_x = c->x;
                new_y = c->y;
            } else if (cost == INT_MAX || m->characters[new_y][new_x] != NULL) {
                int dir_idx = rand() % 8;
                c->dir_x = npcMoveX[dir_idx];
                c->dir_y = npcMoveY[dir_idx];
                new_x = c->x; // Don't move this turn
                new_y = c->y;
            }
        break;
        case SWIMMER:
            new_x = c->x + c->dir_x;
            new_y = c->y + c->dir_y;

            cost = move_cost(c->type, m->t[new_y][new_x].type);

            if (m->characters[new_y][new_x] != NULL && m->characters[new_y][new_x]->type == PC) {
                battled = c;
                new_x = c->x;
                new_y = c->y;
            } else if(cost == INT_MAX || m->characters[new_y][new_x] != NULL || m->t[new_y][new_x].type != TERRAIN_WATER) {
                int dir_idx = rand() % 8;
                c->dir_x = npcMoveX[dir_idx];
                c->dir_y = npcMoveY[dir_idx];
                new_x = c->x;
                new_y = c->y;
            }
        break; 
        default:
            break;
    }

    // Perform the move
    if (new_x != c->x || new_y != c->y) {
        m->characters[c->y][c->x] = NULL;
        c->x = new_x;
        c->y = new_y;
        m->characters[c->y][c->x] = c;
    }

    // Update next turn time
    // If we didn't move (waiting or blocked), we pay the cost of the current tile
    int moveCost = move_cost(c->type, m->t[c->y][c->x].type);
    c->next_turn += moveCost;

    return battled;
}

typedef struct path_node {
    int x;
    int y;
    int cost;
} path_node_t;

static int32_t path_cmp(const void *key, const void *with) {
    return ((const path_node_t *)key)->cost - ((const path_node_t *)with)->cost;
}

void dijkstra_path(map *m, character_type type) {
    heap_t h;
    heap_init(&h, path_cmp, free);

    heap_node_t *nodes[ROW][COLUMN];
    int (*dist)[COLUMN] = (type == HIKER) ? m->hiker_dist : m->rival_dist;


    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++) {
            dist[i][j] = INT_MAX;
            nodes[i][j] = NULL;
            if (m->characters[i][j] && m->characters[i][j]->type == PC) {
                dist[i][j] = 0;
            }
        }
    }

    // Initialize heap with all nodes
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++) {
            path_node_t *pn = new path_node_t;
            pn->x = j;
            pn->y = i;
            pn->cost = dist[i][j];
            nodes[i][j] = heap_insert(&h, pn);
        }
    }

    int neighbor_x[] = {0, 0, 1, -1, 1, -1, 1, -1};
    int neighbor_y[] = {-1, 1, 0, 0, -1, -1, 1, 1};

    while (h.size > 0) {
        path_node_t *u = (path_node_t *)heap_remove_min(&h);
        int ux = u->x;
        int uy = u->y;
        int ucost = u->cost;
        
        // stop if we pulled an unreachable node, but we want to process everything reachable (duh)
        if (ucost == INT_MAX) {
            free(u);
            break; 
        }

        for (int i = 0; i < 8; i++) {
            int vx = ux + neighbor_x[i];
            int vy = uy + neighbor_y[i];

            if (vx >= 0 && vx < COLUMN && vy >= 0 && vy < ROW) {
                int weight = move_cost(type, m->t[vy][vx].type);
                
                if (weight != INT_MAX) {
                    int alt = ucost + weight;
                    if (alt < dist[vy][vx]) {
                        dist[vy][vx] = alt;
                        
                        path_node_t *vn = new path_node_t();
                        vn->x = vx;
                        vn->y = vy;
                        vn->cost = alt;
                        
                        heap_decrease_key(&h, nodes[vy][vx], vn);
                    }
                }
            }
        }
        free(u);
    }
    heap_delete(&h);
}