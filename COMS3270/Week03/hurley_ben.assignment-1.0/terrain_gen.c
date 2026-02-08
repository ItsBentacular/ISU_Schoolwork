#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"

#define ROW 21
#define COLUMN 80


// this function allows the borders to be defined as different values while generating as terrain.
int generate_start(terrain t_type, int map[][]){
        for(int i = 0; i < ROW; i++) {
            for(int j = 0; j < COLUMN; j++) {
                if((i == 0 || i == ROW -1) || (j == 0 || j == COLUMN -1)) {
                    map[i][j] = t_type;
                } else {
                map[i][j] = TERRAIN_EMPTY;
                }
            }
    }
}