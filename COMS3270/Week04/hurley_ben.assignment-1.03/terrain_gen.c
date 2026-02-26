#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "heap.h"
#include "terrain_gen.h"
#include <limits.h>

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
        
        gen_point_t *pt = malloc(sizeof(*pt));
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
        gen_point_t *p = heap_remove_min(h);
        from_x = p->x;
        from_y = p->y;
        from_type = p->type;
        free(p); // We must free the wrapper, be free!

        for(int i = 0; i < 4; i++) {
            int to_x = from_x + neighbor_x[i];
            int to_y = from_y + neighbor_y[i];
            if((to_x < COLUMN - 1 && to_x > 0) && (to_y < ROW - 1 && to_y > 0) && m->t[to_y][to_x].type == TERRAIN_EMPTY) {

                m->t[to_y][to_x].type = from_type;
                
                gen_point_t *next_pt = malloc(sizeof(*next_pt));
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

int generate_roads(map *m, world w, gate g[4]) {
    // finds a random point NOT in the corners for the gates, then draws lines out of # to a certain x or y coord, then draws another line between the ends of the two lines
    // addendum: now also checks if we are in world bounds, then if there is already a map generated in a spot, make the opposite road connect to the new maps road,
    // ex: north connects to new maps south, west connects to east, else generate random roads.
    int n_gate,s_gate,e_gate,w_gate;
    // gate g[4] goes in order of north, south, east, west

    //north map
    if(w.current_y < 400 && w.m[w.current_x][w.current_y + 1] != NULL) {
        n_gate = w.m[w.current_x][w.current_y + 1]->g[1].gate_pos;
    } else {
        n_gate = rand() % (COLUMN -2) + 1;
    }
    //south map
    if(w.current_y > 0 && w.m[w.current_x][w.current_y - 1] != NULL) {
        s_gate = w.m[w.current_x][w.current_y - 1]->g[0].gate_pos;
    } else {
        s_gate = rand() % (COLUMN -2) + 1;
    }
    //east map
    if(w.current_x < 400 && w.m[w.current_x + 1][w.current_y] != NULL) {
        e_gate = w.m[w.current_x + 1][w.current_y]->g[3].gate_pos;
    } else {
        e_gate = rand() % (ROW - 2) + 1;
    }
    //west map
    if(w.current_x > 0 && w.m[w.current_x - 1][w.current_y] != NULL) {
        w_gate = w.m[w.current_x - 1][w.current_y]->g[2].gate_pos;
    } else {
        w_gate = rand() % (ROW - 2) + 1;
    }

    int road_y = (rand() % 10) + 5;
    int road_x = (rand() % 40) + 20;

    // generates the east and west roads.
    for(int i = (w.current_x == 0) ? 1 : 0; i <= road_x; i++) {
        m->t[w_gate][i].type = TERRAIN_ROAD;
    }
    for(int i = (w.current_x == 400) ? COLUMN - 2 : COLUMN - 1; i >= road_x; i--) {
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
    for(int i = (w.current_y == 400) ? 1 : 0; i <= road_y; i++) {
        m->t[i][n_gate].type = TERRAIN_ROAD;
    }
    for(int i = (w.current_y == 0) ? ROW - 2 : ROW - 1; i >= road_y; i--) {
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

    if (w.current_y < 400) m->t[0][n_gate].type = TERRAIN_GATE;
    if (w.current_y > 0) m->t[ROW - 1][s_gate].type = TERRAIN_GATE;
    if (w.current_x < 400) m->t[e_gate][COLUMN - 1].type = TERRAIN_GATE;
    if (w.current_x > 0) m->t[w_gate][0].type = TERRAIN_GATE;

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

                    m->t[space_y-1][space_x].type   = 'C';
                    m->t[space_y-2][space_x].type   = 'C';
                    m->t[space_y-1][space_x+1].type = 'C';
                    m->t[space_y-2][space_x+1].type = 'C';
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
                    
                    m->t[space_y+1][space_x].type   = 'M';
                    m->t[space_y+2][space_x].type   = 'M';
                    m->t[space_y+1][space_x+1].type = 'M';
                    m->t[space_y+2][space_x+1].type = 'M';
                    building_placed = 1;
                }
            }
        }
    }

    return 0;
}

// prints final map of terrain.
int map_print(map *m){

    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COLUMN; j++) {
            if (m->t[i][j].type == TERRAIN_BORDER) {
                printf("%%");
            } else if (m->t[i][j].type == TERRAIN_GATE) {
                printf("#");
            } else if (i == m->pc.y && j == m->pc.x) {
                printf("@");
            } else {
                printf("%c", m->t[i][j].type);
            }
        }
        printf("\n");
    }
    return 0;
}

int move_cost(character_type c, terrain t) {
    switch (c) {
        case PC:
            switch (t) {
                case TERRAIN_ROAD: return 10;
                case TERRAIN_GATE: return 10;
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
        case OTHER:
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

void place_pc(map *m) {
    int placed = 0;
    while (!placed) {
        int x = rand() % (COLUMN - 2) + 1;
        int y = rand() % (ROW - 2) + 1;

        if (m->t[y][x].type == TERRAIN_ROAD && m->t[y][x].type != TERRAIN_GATE) {
            m->pc.x = x;
            m->pc.y = y;
            m->pc.type = PC;
            placed = 1;
        }
    }
}

typedef struct path_node {
    int x;
    int y;
    int cost;
} path_node_t;

static int32_t path_cmp(const void *key, const void *with) {
    return ((const path_node_t *)key)->cost - ((const path_node_t *)with)->cost;
}

void dijkstra_path(map *m, character_type type, int dist[21][80]) {
    heap_t h;
    heap_init(&h, path_cmp, free);

    heap_node_t *nodes[ROW][COLUMN];

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++) {
            dist[i][j] = INT_MAX;
            nodes[i][j] = NULL;
        }
    }

    dist[m->pc.y][m->pc.x] = 0;

    // Initialize heap with all nodes
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++) {
            path_node_t *pn = malloc(sizeof(path_node_t));
            pn->x = j;
            pn->y = i;
            pn->cost = dist[i][j];
            nodes[i][j] = heap_insert(&h, pn);
        }
    }

    int neighbor_x[] = {0, 0, 1, -1, 1, -1, 1, -1};
    int neighbor_y[] = {-1, 1, 0, 0, -1, -1, 1, 1};

    while (h.size > 0) {
        path_node_t *u = heap_remove_min(&h);
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
                        
                        path_node_t *vn = malloc(sizeof(path_node_t));
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

void print_dist_map(int dist[21][80]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++) {
            if (dist[i][j] == INT_MAX) {
                printf("   ");
            } else {
                printf("%02d ", dist[i][j] % 100);
            }
        }
        printf("\n");
    }
}