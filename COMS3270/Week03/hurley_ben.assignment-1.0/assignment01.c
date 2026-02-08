#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"
#include "terrain_gen.h"

world w;

int initialize_world(world *w) {
    for(int i = 0; i < 401; i++) {
        for(int j = 0; j < 401; j++) {
            w->m[i][j] = NULL;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    initialize_world(&w);

    int current_x = 200;
    int current_y = 200;

    terrain seeds[7] = {TERRAIN_WATER, TERRAIN_CLEAR, TERRAIN_CLEAR, TERRAIN_GRASS, TERRAIN_GRASS, TERRAIN_ROCK, TERRAIN_TREES};

    queue q;

    char input = ' ';

    while(input != 'q') {
        if(w.m[current_x][current_y] == NULL) {
            map *m = malloc(sizeof(map));

            generate_start(TERRAIN_ROCK, m, seeds, &q);
            generate_roads(m);
            generate_builds(m);

            w.m[current_x][current_y] = m;
        }
        map_print(w.m[current_x][current_y]);
        printf("x: %d y: %d\n", current_x - 200, current_y - 200);
        scanf("%c", &input);
        switch(input) {
            case 'n':
                current_y += 1;
            break;
            case 's':
                current_y -= 1;
            break;
            case 'e':
                current_x += 1;
            break;
            case 'w':
                current_x -= 1;
            break;
            case 'f':
                printf("where to?\n");
                scanf("%d %d", &current_x, &current_y);
                current_x += 200;
                current_y += 200;
            break;
        }

    }
    for(int i = 0; i < 401; i++) {
        for(int j = 0; j < 401; j++) {
            if(w.m[i][j] != NULL) {
                free(w.m[i][j]);
            }
        }
    }
}