#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"

#define ROW 21
#define COLUMN 80

int map[ROW][COLUMN];

int main(int argc, char *argv[]) {
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

    queue q;
    queue_init(&q);
    for(int i = 0; i < 7; i++) {
        int ry = rand() % 19 + 1;
        int rx = rand() % 79 + 1;
        int current_type;
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
    for(int i = 0; i < road_x; i++) {
        map[w_gate][i] = TERRAIN_ROAD;
    }
    for(int i = COLUMN; i >= road_x; i--) {
        map[e_gate][i] = TERRAIN_ROAD;
    }
    for(int i = 0; i < road_y; i++) {
        map[i][n_gate] = TERRAIN_ROAD;
    }
    for(int i = ROW; i >= road_y; i--) {
        map[i][s_gate] = TERRAIN_ROAD;
    }

    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COLUMN; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }

}