#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"

#define ROW 21
#define COLUMN 80

int map[ROW][COLUMN];

int main(int argc, char *argv[]) {
    //seeds the random values then sets the border to rock and middle to empty with enum listed in queue.h
    srand(time(NULL));
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COLUMN; j++) {
            if((i == 0 || i == ROW -1) || (j == 0 || j == COLUMN -1)) {
                map[i][j] = TERRAIN_ROCK;
            } else {
            map[i][j] = TERRAIN_EMPTY;
            }
        }
    }

    //creates a queue structure, queues terrain types at random positions, then places the terrain onto the map.
    queue q;
    queue_init(&q);
    for(int i = 0; i < 7; i++) {
        int ry = rand() % 19 + 1;
        int rx = rand() % 79 + 1;
        int current_type;
        // not sure how efficient this switch case is, but it does a pretty good job at allowing the queue to be semi-random by randomizing the ry and rx values within the loop while keeping the terrain types the same.
        switch(i){
            case 0: current_type = TERRAIN_WATER; queue_enqueue(&q, rx, ry, current_type); break;

            case 1: current_type = TERRAIN_CLEAR; queue_enqueue(&q, rx, ry, current_type); break;

            case 2: current_type = TERRAIN_CLEAR; queue_enqueue(&q, rx, ry, current_type); break;

            case 3: current_type = TERRAIN_GRASS; queue_enqueue(&q, rx, ry, current_type); break;

            case 4: current_type = TERRAIN_GRASS; queue_enqueue(&q, rx, ry, current_type); break;

            case 5: current_type = TERRAIN_ROCK; queue_enqueue(&q, rx, ry, current_type); break;

            case 6: current_type = TERRAIN_TREES; queue_enqueue(&q, rx, ry, current_type); break;
        }

        map[ry][rx] = current_type;
    }

    // generates more terrain around the 'seeds' by checking if the terrains surrounding area is in bounds, then puts the same type of terrain around it, and requeues the new terrain onto the queue and repeats until map is full.
    while(queue_getSize(&q) != 0) {
        int from_x, from_y;
        terrain from_type;
        int neighbor_x[] = {0,0,1,-1};
        int neighbor_y[] = {-1,1,0,0};

        queue_dequeue(&q, &from_x, &from_y, &from_type);

        for(int i = 0; i < 4; i++) {
            int to_x = from_x + neighbor_x[i];
            int to_y = from_y + neighbor_y[i];
            if((to_x < COLUMN - 1 && to_x > 0) && (to_y < ROW - 1 && to_y > 0) && map[to_y][to_x] == TERRAIN_EMPTY) {

                map[to_y][to_x] = from_type;
                queue_enqueue(&q,to_x,to_y,from_type);
            }
        }
    }

    // finds a random point NOT in the corners for the gates, then draws lines out of # to a certain x or y coord, then draws another line between the ends of the two lines
    int n_gate = rand() % (COLUMN -2) + 1;
    int s_gate = rand() % (COLUMN -2) + 1;
    int e_gate = rand() % (ROW - 2) + 1;
    int w_gate = rand() % (ROW - 2) + 1;

    map[0][n_gate] = TERRAIN_ROAD;
    map[ROW - 1][s_gate] = TERRAIN_ROAD;
    map[w_gate][0] = TERRAIN_ROAD;
    map[e_gate][COLUMN - 1] = TERRAIN_ROAD;

    int road_y = (rand() % 10) + 5;
    int road_x = (rand() % 40) + 20;

    // generates the east and west roads.
    for(int i = 0; i <= road_x; i++) {
        map[w_gate][i] = TERRAIN_ROAD;
    }
    for(int i = COLUMN - 1; i >= road_x; i--) {
        map[e_gate][i] = TERRAIN_ROAD;
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
        map[i][road_x] = TERRAIN_ROAD;
    }

    // generates the north and south roads.
    for(int i = 0; i <= road_y; i++) {
        map[i][n_gate] = TERRAIN_ROAD;
    }
    for(int i = ROW - 1; i >= road_y; i--) {
        map[i][s_gate] = TERRAIN_ROAD;
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
        map[road_y][i] = TERRAIN_ROAD;
    }

    // generates the pokecenters (C) in a open area on the map that is attached to a road.
    int space_x, space_y;
    int building_placed = 0;
    while(building_placed == 0) {
        space_x = rand() % COLUMN + 1;
        space_y = rand() % ROW + 1;

        if(map[space_y][space_x] == TERRAIN_ROAD && space_y > 3 && space_y < ROW - 3 && space_x > 3 && space_x < COLUMN - 3) {
                if (map[space_y-1][space_x] != TERRAIN_ROAD && map[space_y-2][space_x] != TERRAIN_ROAD && map[space_y-1][space_x+1] != TERRAIN_ROAD && map[space_y-2][space_x+1] != TERRAIN_ROAD) {

                    map[space_y-1][space_x]   = 'C';
                    map[space_y-2][space_x]   = 'C';
                    map[space_y-1][space_x+1] = 'C';
                    map[space_y-2][space_x+1] = 'C';
                    building_placed = 1;
            }
        }
    }

    // generates the pokemarts (M) on the opposite side of the pokecenters, same rules follow.
    building_placed = 0;
        while(building_placed == 0) {
            space_x = rand() % (COLUMN - 1);
            space_y = rand() % (ROW - 1);

            if(map[space_y][space_x] == TERRAIN_ROAD && space_y > 3 && space_y < ROW - 3 && space_x > 3 && space_x < COLUMN - 3) {

                if (map[space_y+1][space_x] != TERRAIN_ROAD && map[space_y+1][space_x] != 'C' && map[space_y+2][space_x] != TERRAIN_ROAD && map[space_y+2][space_x] != 'C' &&
                    map[space_y+1][space_x+1] != TERRAIN_ROAD && map[space_y+1][space_x+1] != 'C' &&
                    map[space_y+2][space_x+1] != TERRAIN_ROAD && map[space_y+2][space_x+1] != 'C') {
                    
                    map[space_y+1][space_x]   = 'M';
                    map[space_y+2][space_x]   = 'M';
                    map[space_y+1][space_x+1] = 'M';
                    map[space_y+2][space_x+1] = 'M';
                    building_placed = 1;
                }
            }
        }



    // prints final map of terrain.
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COLUMN; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
    //printf("building is at: %d %d\n", space_x, space_y);

}