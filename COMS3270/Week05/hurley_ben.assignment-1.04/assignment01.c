#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "heap.h"
#include "terrain_gen.h"
#include <string.h>

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

    heap_t h;
    heap_t npc;


    // same world gen code, if it ain't broke don't fix it.

    int man_dis = (abs(current_x - 200) + abs(current_y - 200));
    w.current_x = current_x;
    w.current_y = current_y;

    if(w.m[current_x][current_y] == NULL) {
        map * m = calloc(1, sizeof(map));

        heap_init(&npc, character_cmp, NULL);

        generate_start(TERRAIN_BORDER, m, seeds, &h);
        generate_roads(m,w,m->g);
        generate_builds(m, man_dis);
        place_pc(m, &npc);
        for(int i = 1; i < argc; i++) {
            if(strcmp(argv[i], "--numtrainers") == 0) {
                if(i + 1 < argc) {
                    place_npc(m, atoi(argv[i + 1]), &npc);
                    i++;
                } else {
                    printf("--numtrainers requires a integer value after.\n");
                    return -1;
                }
            }
        }
        if(argc < 2) {
            place_npc(m,(rand() % (10 - 2) + 2), &npc);
        }

        w.m[current_x][current_y] = m;
    }

    map *cur_map = w.m[current_x][current_y];

    // loop for making people move
    while(1) {
        character *c = (character *)heap_remove_min(&npc);

        if(c->type == PC) {
            map_print(cur_map);
            usleep(150000);
            
            // move pc, update maps, and loop npc queue
            move_npc(c, cur_map);
            dijkstra_path(cur_map, HIKER);
            dijkstra_path(cur_map, RIVAL);
            
            heap_insert(&npc, c);
        } else {
            move_npc(c, cur_map);
            heap_insert(&npc, c);
        }
    }


    // old world code, the future is NOW

    /*while(1) {
        // the manhattan distance (is that like the bomb?)
        int man_dis = (abs(current_x - 200) + abs(current_y - 200));
        w.current_x = current_x;
        w.current_y = current_y;

        if(w.m[current_x][current_y] == NULL) {
            map *m = calloc(1, sizeof(map));

            heap_init(&npc, character_cmp, NULL);

            generate_start(TERRAIN_BORDER, m, seeds, &h);
            generate_roads(m,w,m->g);
            generate_builds(m, man_dis);
            place_pc(m, &npc);
            if(argc < 2) {
                place_npc(m,(rand() % (10 - 2) + 2), &npc);
            } else {
                place_npc(m, atoi(argv[1]), &npc);
            }

            w.m[current_x][current_y] = m;
        }
        map_print(w.m[current_x][current_y]);
        dijkstra_path(w.m[current_x][current_y], HIKER);
        dijkstra_path(w.m[current_x][current_y], RIVAL);

        map *cur_map = w.m[current_x][current_y];
        for(int i = 0; i < 21; i++) {
            for(int j = 0; j < 80; j++) {
                if(cur_map->characters[i][j] != NULL) {
                    move_npc(cur_map->characters[i][j], cur_map);
                }
            }
        }


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
            case 'p':
            break;
        }

    } */
    for(int i = 0; i < 401; i++) {
        for(int j = 0; j < 401; j++) {
            if(w.m[i][j] != NULL) {
                free(w.m[i][j]);
            }
        }
    }
}