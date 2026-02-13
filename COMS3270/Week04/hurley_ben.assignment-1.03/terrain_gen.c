#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"
#include "terrain_gen.h"

#define ROW 21
#define COLUMN 80


// this function allows the borders to be defined as different values while generating as terrain.
int generate_start(terrain border, map *m, terrain seeds[7], queue *q){
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
    queue_init(q);
    for(int i = 0; i < 7; i++) {
        int ry = rand() % 19 + 1;
        int rx = rand() % 79 + 1;
        // not sure how efficient this switch case is, but it does a pretty good job at allowing the queue to be semi-random by randomizing the ry and rx values within the loop while keeping the terrain types the same.
        switch(i){
            case 0: queue_enqueue(q, rx, ry, seeds[0]); break;

            case 1: queue_enqueue(q, rx, ry, seeds[1]); break;

            case 2: queue_enqueue(q, rx, ry, seeds[2]); break;

            case 3: queue_enqueue(q, rx, ry, seeds[3]); break;

            case 4: queue_enqueue(q, rx, ry, seeds[4]); break;

            case 5: queue_enqueue(q, rx, ry, seeds[5]); break;

            case 6: queue_enqueue(q, rx, ry, seeds[6]); break;
        }

        m->t[ry][rx].type = seeds[i];
    }

    // generates more terrain around the 'seeds' by checking if the terrains surrounding area is in bounds, then puts the same type of terrain around it, and requeues the new terrain onto the queue and repeats until map is full.
    while(queue_getSize(q) != 0) {
        int from_x, from_y;
        terrain from_type;
        int neighbor_x[] = {0,0,1,-1};
        int neighbor_y[] = {-1,1,0,0};

        queue_dequeue(q, &from_x, &from_y, &from_type);

        for(int i = 0; i < 4; i++) {
            int to_x = from_x + neighbor_x[i];
            int to_y = from_y + neighbor_y[i];
            if((to_x < COLUMN - 1 && to_x > 0) && (to_y < ROW - 1 && to_y > 0) && m->t[to_y][to_x].type == TERRAIN_EMPTY) {

                m->t[to_y][to_x].type = from_type;
                queue_enqueue(q,to_x,to_y,from_type);
            }
        }
    }

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

    m->t[0][n_gate].type = TERRAIN_ROAD;
    m->t[ROW - 1][s_gate].type = TERRAIN_ROAD;
    m->t[w_gate][0].type = TERRAIN_ROAD;
    m->t[e_gate][COLUMN - 1].type = TERRAIN_ROAD;

    int road_y = (rand() % 10) + 5;
    int road_x = (rand() % 40) + 20;

    // generates the east and west roads.
    for(int i = 0; i <= road_x; i++) {
        m->t[w_gate][i].type = TERRAIN_ROAD;
    }
    for(int i = COLUMN - 1; i >= road_x; i--) {
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
    for(int i = 0; i <= road_y; i++) {
        m->t[i][n_gate].type = TERRAIN_ROAD;
    }
    for(int i = ROW - 1; i >= road_y; i--) {
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
            printf("%c", m->t[i][j].type);
        }
        printf("\n");
    }
    return 0;
}