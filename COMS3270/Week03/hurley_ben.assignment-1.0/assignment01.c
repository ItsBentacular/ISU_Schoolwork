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
        // the manhattan distance (is that like the bomb?)
        int man_dis = (abs(current_x - 200) + abs(current_y - 200));
        w.current_x = current_x;
        w.current_y = current_y;

        if(w.m[current_x][current_y] == NULL) {
            map *m = malloc(sizeof(map));

            generate_start(TERRAIN_ROCK, m, seeds, &q);
            generate_roads(m,w,m->g);
            generate_builds(m, man_dis);

            w.m[current_x][current_y] = m;
        }
        map_print(w.m[current_x][current_y]);
        printf("x: %d y: %d\n", current_x - 200, current_y - 200);
        scanf(" %c", &input); // Added space to consume newline
        switch(input) {
            case 'n':
                if (current_y < 400) current_y += 1;
            break;
            case 's':
                if (current_y > 0) current_y -= 1;
            break;
            case 'e':
                if (current_x < 400) current_x += 1;
            break;
            case 'w':
                if (current_x > 0) current_x -= 1;
            break;
            case 'f':
                int tx, ty;
                printf("where to?\n");
                if (scanf("%d %d", &tx, &ty) == 2) {
                    if (tx >= -200 && tx <= 200 && ty >= -200 && ty <= 200) {
                        current_x = tx + 200;
                        current_y = ty + 200;
                    } else {
                        printf("Coordinates out of bounds (-200 to 200)\n");
                    }
                } else {
                    printf("Invalid input\n");
                }
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