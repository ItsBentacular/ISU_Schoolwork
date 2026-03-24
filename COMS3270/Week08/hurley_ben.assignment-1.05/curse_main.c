#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "heap.h"
#include "poke_logic.h"
#include <string.h>
#include <ncurses.h>
#include <limits.h>


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

    int runGame = 1;


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
        generate_names(m);
        if(argc < 2) {
            place_npc(m,(rand() % (10 - 2) + 2), &npc);
        }

        w.m[current_x][current_y] = m;
    }

    map *cur_map = w.m[current_x][current_y];

    // loop for making people move, ncurses logic starts here
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    start_color();
    //use_default_colors();
    short COLOR_BROWN = 10;
    init_color(COLOR_BROWN, 545, 270, 74);
    short COLOR_GREY = 11;
    init_color(COLOR_GREY, 650,650,600);

    init_pair(1, COLOR_BLUE, COLOR_BLACK); // water
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // grass path
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); // road
    init_pair(4, COLOR_GREY, COLOR_BLACK); // mountain
    init_pair(5, COLOR_BROWN, COLOR_BLACK); // tree
    init_pair(6, COLOR_WHITE, COLOR_BLACK); // poke-buildings

    while(runGame) {
        character *c = (character *)heap_remove_min(&npc);

        if(c->type == PC) {
            map_print(cur_map);
            
            // Clear the reserved lines (0, 22, 23) to prevent leftover character artifacts
            mvhline(0, 0, ' ', 80);
            mvhline(22, 0, ' ', 80);
            mvhline(23, 0, ' ', 80);
            
            // Render the Message display (top line) and Status information (bottom 2 lines)
            mvprintw(0, 0, "A wild foobar appears!");
            mvprintw(22, 0, " ");
            mvprintw(23, 0, "Use y/k/u/l/n/j/b/h to move. Press 'Q' to quit.");
            refresh();
            
            int tookTurn = 0;

            while(!tookTurn && runGame) {
                int input = getch();

                switch(input) {
                    case 'Q':
                    runGame = 0;
                    break;
                    case 'y': case '7':
                    if(c->y - 1 > 0 && c->x - 1 > 0 && move_cost(c->type, cur_map->t[c->y - 1][c->x - 1].type) != INT_MAX) {
                        // Clear the old position in the array
                        cur_map->characters[c->y][c->x] = NULL;
                        // Update the coordinates
                        c->x -= 1;
                        c->y -= 1;
                        // Place the character into the new array position
                        cur_map->characters[c->y][c->x] = c;
                    }
                    tookTurn = 1;
                    break;
                    case 'k': case '8':
                    if(c->y - 1 > 0 && move_cost(c->type, cur_map->t[c->y - 1][c->x].type) != INT_MAX) {
                        cur_map->characters[c->y][c->x] = NULL;
                        c->x += 0;
                        c->y -= 1;
                        cur_map->characters[c->y][c->x] = c;
                    }
                    tookTurn = 1;
                    break;
                    case 'u': case '9':
                    if(c->y - 1 > 0 && c->x + 1 < 79 && move_cost(c->type, cur_map->t[c->y - 1][c->x + 1].type) != INT_MAX) {
                        cur_map->characters[c->y][c->x] = NULL;
                        c->x += 1;
                        c->y -= 1;
                        cur_map->characters[c->y][c->x] = c;
                    }
                    tookTurn = 1;
                    break;
                    case 'l': case '6':
                    if(c->x + 1 < 79 && move_cost(c->type, cur_map->t[c->y][c->x + 1].type) != INT_MAX) {
                        cur_map->characters[c->y][c->x] = NULL;
                        c->x += 1;
                        cur_map->characters[c->y][c->x] = c;
                    }
                    tookTurn = 1;
                    break;
                    case 'n': case '3':
                    if(c->y + 1 < 20 && c->x + 1 < 79 && move_cost(c->type, cur_map->t[c->y + 1][c->x + 1].type) != INT_MAX) {
                        cur_map->characters[c->y][c->x] = NULL;
                        c->x += 1;
                        c->y += 1;
                        cur_map->characters[c->y][c->x] = c;
                    }
                    tookTurn = 1;
                    break;
                    case 'j': case '2':
                    if(c->y + 1 < 20 && move_cost(c->type, cur_map->t[c->y + 1][c->x].type) != INT_MAX) {
                        cur_map->characters[c->y][c->x] = NULL;
                        c->y += 1;
                        cur_map->characters[c->y][c->x] = c;
                    }
                    tookTurn = 1;
                    break;
                    case 'b': case '1':
                    if(c->y + 1 < 20 && c->x - 1 > 0 && move_cost(c->type, cur_map->t[c->y + 1][c->x - 1].type) != INT_MAX) {
                        cur_map->characters[c->y][c->x] = NULL;
                        c->x -= 1;
                        c->y += 1;
                        cur_map->characters[c->y][c->x] = c;
                    }
                    tookTurn = 1;
                    break;
                    case 'h': case '4':
                    if(c->x - 1 > 0 && move_cost(c->type, cur_map->t[c->y][c->x - 1].type) != INT_MAX) {
                        cur_map->characters[c->y][c->x] = NULL;
                        c->x -= 1;
                        cur_map->characters[c->y][c->x] = c;
                    }
                    tookTurn = 1;
                    break;
                    case '>':
                    break;
                    case '5': case '.': case ' ':
                    tookTurn = 1;
                    break;
                    case 't':
                    // 1. Clear the entire screen (rows 0 through 23)
                    for(int i = 0; i < 24; i++) {
                        mvhline(i, 0, ' ', 80);
                    }
                    
                    // 2. Draw some placeholder menu text and refresh the screen
                    mvprintw(0, 0, "Trainer List");
                    mvprintw(1, 0, "--------------");
                    int placedNPC = 1;
                    for(int i = 0; i < 21; i++) {
                        for(int j = 0; j < 80; j++) {
                            if(cur_map->characters[i][j] != NULL && cur_map->characters[i][j]->type != PC) {
                                placedNPC++;
                                mvprintw(placedNPC, 0, " %c  %s    X: %d, Y: %d ", cur_map->characters[i][j]->type, cur_map->characters[i][j]->charName, cur_map->characters[i][j]->x, cur_map->characters[i][j]->y);
                            }
                        }

                    }
                    refresh();

                    // 3. Wait for the user to press the Escape key (ASCII 27)
                    int menu_input = 0;
                    while(menu_input != 27) {
                        menu_input = getch();
                    }

                    // 4. Redraw the main game map and UI before returning to the game loop
                    map_print(cur_map);
                    mvhline(0, 0, ' ', 80);
                    mvprintw(0, 0, "A wild foobar appears!");
                    refresh();
                    break;
                    case KEY_UP:
                    break;
                    case KEY_DOWN:
                    break;
                    default:
                    break;
                }
            }

            if(runGame) {
                move_npc(c, cur_map);
                dijkstra_path(cur_map, HIKER);
                dijkstra_path(cur_map, RIVAL);

                heap_insert(&npc, c);
            }
        } else {
            move_npc(c, cur_map);
            heap_insert(&npc, c);
        }
    }
    endwin();

    for(int i = 0; i < 401; i++) {
        for(int j = 0; j < 401; j++) {
            if(w.m[i][j] != NULL) {
                free(w.m[i][j]);
            }
        }
    }
}