#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "heap.h"
#include "poke_logic.hpp"
#include "poke_data.hpp"
#include <string.h>
#include <ncurses.h>
#include <limits.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>


world w;

void battle_trainer(character *npc) {
    if (npc->isDefeated) {
        return; // Already defeated, don't battle again.
    }
    WINDOW *battle_win = newwin(24, 80, 0, 0);
    box(battle_win, 0, 0);
    mvwprintw(battle_win, 10, 25, "Battle with %s!", npc->charName);
    mvwprintw(battle_win, 12, 25, "Press ESC to exit battle.");
    wrefresh(battle_win);
    
    int ch;
    while((ch = getch()) != 27) {
    }
    npc->isDefeated = 1;
    delwin(battle_win);
    clear();
}

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
    int debug = 0;

    std::ifstream file;


    terrain seeds[7] = {TERRAIN_WATER, TERRAIN_CLEAR, TERRAIN_CLEAR, TERRAIN_GRASS, TERRAIN_GRASS, TERRAIN_ROCK, TERRAIN_TREES};

    heap_t h;
    heap_t npc;


    // same world gen code, if it ain't broke don't fix it.

    int man_dis = (abs(current_x - 200) + abs(current_y - 200));
    w.current_x = current_x;
    w.current_y = current_y;

    if(w.m[current_x][current_y] == NULL) {
        map * m = new map();

        heap_init(&npc, character_cmp, NULL);

        generate_start(TERRAIN_BORDER, m, seeds, &h);
        generate_roads(m,&w,m->g);
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
            if(strcmp(argv[i], "--debug") == 0) {
            debug = 1;
            } else {
                // assignment 1.07 case is here, if no other start params, check for file name.
                loadPokeData(file, argv[i]);
                return 0;
            }
        }
        if(argc < 2 || debug) {
            place_npc(m,(rand() % (10 - 2) + 2), &npc);
        }
        generate_names(m);

        w.m[current_x][current_y] = m;
    }

    map *cur_map = w.m[current_x][current_y];

    // loop for making people move, ncurses logic starts here
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    scrollok(stdscr,TRUE);
    WINDOW *trainerPad;
    if(has_colors()) {
        start_color();
    }
    short COLOR_BROWN = 10;
    init_color(COLOR_BROWN, 545, 270, 74);
    short COLOR_GREY = 11;
    init_color(COLOR_GREY, 650,650,600);
    short COLOR_GRASS_BACK = 10;
    init_color(COLOR_GRASS_BACK, 545, 270, 74);
    short COLOR_OCEAN = 11;
    init_color(COLOR_OCEAN, 650,650,600);


    init_pair(1, COLOR_BLUE, COLOR_BLUE); // water
    init_pair(2, COLOR_GREEN, COLOR_GREEN); // grass path
    init_pair(3, COLOR_YELLOW, COLOR_YELLOW); // road
    init_pair(4, COLOR_GREY, COLOR_GREY); // mountain
    init_pair(5, COLOR_BROWN, COLOR_BROWN); // tree
    init_pair(6, COLOR_WHITE, COLOR_RED); // poke-buildings

    const char *plainMessage = "A wild foobar has appeared!";
    const char *debugMessage = "Debug Mode Active.";
    while(runGame) {
        character *c = (character *)heap_remove_min(&npc);
        if (c == NULL) {
            break; // Safely exit or handle empty heap to prevent a crash
        }

        if(c->type == PC) {
            map_print(cur_map);
            
            // Clear the reserved lines (0, 22, 23) to prevent leftover character artifacts
            mvhline(0, 0, ' ', 80);
            mvhline(22, 0, ' ', 80);
            mvhline(23, 0, ' ', 80);
            
            // Render the Message display (top line) and Status information (bottom 2 lines)
            if(debug == 0) {
                mvprintw(0, 0, "%s", plainMessage);             
            } else {
                mvprintw(0, 0, "%s", debugMessage);  
            }
            mvprintw(22, 0, " ");
            mvprintw(23, 0, "Use y/k/u/l/n/j/b/h to move. Press 'Q' to quit.");
            refresh();
            
            map *old_map = cur_map;
            int tookTurn = 0;

            while(!tookTurn && runGame) {
                int input = getch();

                switch(input) {
                    case 'Q':
                    runGame = 0;
                    break;
                    case 'y': case '7':
                    if(move_cost(c->type, cur_map->t[c->y - 1][c->x - 1].type) != INT_MAX) {
                        if(cur_map->characters[c->y - 1][c->x - 1] != NULL && !cur_map->characters[c->y - 1][c->x - 1]->isDefeated) {
                            battle_trainer(cur_map->characters[c->y - 1][c->x - 1]);
                        } else {
                            character *swap = cur_map->characters[c->y - 1][c->x - 1];
                            cur_map->characters[c->y][c->x] = swap;
                            if (swap) {
                                swap->x = c->x;
                                swap->y = c->y;
                            }
                            c->x -= 1;
                            c->y -= 1;
                            cur_map->characters[c->y][c->x] = c;
                        }
                    }
                    tookTurn = 1;
                    break;
                    case 'k': case '8':
                    if(cur_map->t[c->y - 1][c->x].type == TERRAIN_GATE && current_y < 400) {
                        current_y += 1;
                        cur_map->characters[c->y][c->x] = NULL;
                        man_dis = (abs(current_x - 200) + abs(current_y - 200));
                        w.current_y = current_y;
                        w.current_x = current_x;
                        if(w.m[current_x][current_y] == NULL) {
                            map* m = new map();

                            generate_start(TERRAIN_BORDER, m, seeds, &h);
                            generate_roads(m,&w,m->g);
                            generate_builds(m, man_dis);

                            place_npc(m,(rand() % (10 - 2) + 2), &npc);
                            generate_names(m);
                            w.m[current_x][current_y] = m;
                            
                        }
                        cur_map = w.m[current_x][current_y];
                        c->y += 19;
                        cur_map->characters[c->y][c->x] = c;

                    }
                    if(move_cost(c->type, cur_map->t[c->y - 1][c->x].type) != INT_MAX) {
                        if(cur_map->characters[c->y - 1][c->x] != NULL && !cur_map->characters[c->y - 1][c->x]->isDefeated) {
                            battle_trainer(cur_map->characters[c->y - 1][c->x]);
                        } else {
                            character *swap = cur_map->characters[c->y - 1][c->x];
                            cur_map->characters[c->y][c->x] = swap;
                            if (swap) {
                                swap->x = c->x;
                                swap->y = c->y;
                            }
                            c->x += 0;
                            c->y -= 1;
                            cur_map->characters[c->y][c->x] = c;
                        }
                    }
                    tookTurn = 1;
                    break;
                    case 'u': case '9':
                    if(move_cost(c->type, cur_map->t[c->y - 1][c->x + 1].type) != INT_MAX) {
                        if(cur_map->characters[c->y - 1][c->x + 1] != NULL && !cur_map->characters[c->y - 1][c->x + 1]->isDefeated) {
                            battle_trainer(cur_map->characters[c->y - 1][c->x + 1]);
                        } else {
                            character *swap = cur_map->characters[c->y - 1][c->x + 1];
                            cur_map->characters[c->y][c->x] = swap;
                            if (swap) {
                                swap->x = c->x;
                                swap->y = c->y;
                            }
                            c->x += 1;
                            c->y -= 1;
                            cur_map->characters[c->y][c->x] = c;
                        }
                    }
                    tookTurn = 1;
                    break;
                    case 'l': case '6':
                    if(cur_map->t[c->y][c->x + 1].type == TERRAIN_GATE && current_x < 400) {
                        current_x += 1;
                        cur_map->characters[c->y][c->x] = NULL;
                        man_dis = (abs(current_x - 200) + abs(current_y - 200));
                        w.current_y = current_y;
                        w.current_x = current_x;
                        if(w.m[current_x][current_y] == NULL) {
                            map* m = new map();

                            generate_start(TERRAIN_BORDER, m, seeds, &h);
                            generate_roads(m,&w,m->g);
                            generate_builds(m, man_dis);

                            place_npc(m,(rand() % (10 - 2) + 2), &npc);
                            generate_names(m);
                            w.m[current_x][current_y] = m;
                            
                        }
                        cur_map = w.m[current_x][current_y];
                        c->x = 1;
                        cur_map->characters[c->y][c->x] = c;

                    }
                    if(move_cost(c->type, cur_map->t[c->y][c->x + 1].type) != INT_MAX) {
                        if(cur_map->characters[c->y][c->x + 1] != NULL && !cur_map->characters[c->y][c->x + 1]->isDefeated) {
                            battle_trainer(cur_map->characters[c->y][c->x + 1]);
                        } else {
                            character *swap = cur_map->characters[c->y][c->x + 1];
                            cur_map->characters[c->y][c->x] = swap;
                            if (swap) {
                                swap->x = c->x;
                                swap->y = c->y;
                            }
                            c->x += 1;
                            cur_map->characters[c->y][c->x] = c;
                        }
                    }
                    tookTurn = 1;
                    break;
                    case 'n': case '3':
                    if(move_cost(c->type, cur_map->t[c->y + 1][c->x + 1].type) != INT_MAX) {
                        if(cur_map->characters[c->y + 1][c->x + 1] != NULL && !cur_map->characters[c->y + 1][c->x + 1]->isDefeated) {
                            battle_trainer(cur_map->characters[c->y + 1][c->x + 1]);
                        } else {
                            character *swap = cur_map->characters[c->y + 1][c->x + 1];
                            cur_map->characters[c->y][c->x] = swap;
                            if (swap) {
                                swap->x = c->x;
                                swap->y = c->y;
                            }
                            c->x += 1;
                            c->y += 1;
                            cur_map->characters[c->y][c->x] = c;
                        }
                    }
                    tookTurn = 1;
                    break;
                    case 'j': case '2':
                    if(cur_map->t[c->y + 1][c->x].type == TERRAIN_GATE && current_y > 0) {
                        current_y -= 1;
                        cur_map->characters[c->y][c->x] = NULL;
                        man_dis = (abs(current_x - 200) + abs(current_y - 200));
                        w.current_y = current_y;
                        w.current_x = current_x;
                        if(w.m[current_x][current_y] == NULL) {
                            map* m = new map();

                            generate_start(TERRAIN_BORDER, m, seeds, &h);
                            generate_roads(m,&w,m->g);
                            generate_builds(m, man_dis);

                            place_npc(m,(rand() % (10 - 2) + 2), &npc);
                            generate_names(m);
                            w.m[current_x][current_y] = m;
                            
                        }
                        cur_map = w.m[current_x][current_y];
                        c->y -= 19;
                        cur_map->characters[c->y][c->x] = c;

                    }
                    if(move_cost(c->type, cur_map->t[c->y + 1][c->x].type) != INT_MAX) {
                        if(cur_map->characters[c->y + 1][c->x] != NULL && !cur_map->characters[c->y + 1][c->x]->isDefeated) {
                            battle_trainer(cur_map->characters[c->y + 1][c->x]);
                        } else {
                            character *swap = cur_map->characters[c->y + 1][c->x];
                            cur_map->characters[c->y][c->x] = swap;
                            if (swap) {
                                swap->x = c->x;
                                swap->y = c->y;
                            }
                            c->y += 1;
                            cur_map->characters[c->y][c->x] = c;
                        }
                    }
                    tookTurn = 1;
                    break;
                    case 'b': case '1':
                    if(move_cost(c->type, cur_map->t[c->y + 1][c->x - 1].type) != INT_MAX) {
                        if(cur_map->characters[c->y + 1][c->x - 1] != NULL && !cur_map->characters[c->y + 1][c->x - 1]->isDefeated) {
                            battle_trainer(cur_map->characters[c->y + 1][c->x - 1]);
                        } else {
                            character *swap = cur_map->characters[c->y + 1][c->x - 1];
                            cur_map->characters[c->y][c->x] = swap;
                            if (swap) {
                                swap->x = c->x;
                                swap->y = c->y;
                            }
                            c->x -= 1;
                            c->y += 1;
                            cur_map->characters[c->y][c->x] = c;
                        }
                    }
                    tookTurn = 1;
                    break;
                    case 'h': case '4':
                    if(cur_map->t[c->y][c->x - 1].type == TERRAIN_GATE && current_x > 0) {
                        current_x -= 1;
                        cur_map->characters[c->y][c->x] = NULL;
                        man_dis = (abs(current_x - 200) + abs(current_y - 200));
                        w.current_y = current_y;
                        w.current_x = current_x;
                        if(w.m[current_x][current_y] == NULL) {
                            map* m = new map();

                            generate_start(TERRAIN_BORDER, m, seeds, &h);
                            generate_roads(m,&w,m->g);
                            generate_builds(m, man_dis);

                            place_npc(m,(rand() % (10 - 2) + 2), &npc);
                            generate_names(m);
                            w.m[current_x][current_y] = m;
                            
                        }
                        cur_map = w.m[current_x][current_y];
                        c->x = 78;
                        cur_map->characters[c->y][c->x] = c;
                    }
                    if(move_cost(c->type, cur_map->t[c->y][c->x - 1].type) != INT_MAX) {
                        if(cur_map->characters[c->y][c->x - 1] != NULL && !cur_map->characters[c->y][c->x - 1]->isDefeated) {
                            battle_trainer(cur_map->characters[c->y][c->x - 1]);
                        } else {
                            character *swap = cur_map->characters[c->y][c->x - 1];
                            cur_map->characters[c->y][c->x] = swap;
                            if (swap) {
                                swap->x = c->x;
                                swap->y = c->y;
                            }
                            c->x -= 1;
                            cur_map->characters[c->y][c->x] = c;
                        }
                    }
                    tookTurn = 1;
                    break;
                    case '>':
                    if(cur_map->t[c->y][c->x].type == TERRAIN_POKEC || cur_map->t[c->y][c->x].type == TERRAIN_POKEM) {
                        mvhline(0, 0, ' ', 80);
                        if(cur_map->t[c->y][c->x].type == TERRAIN_POKEC) {
                            mvprintw(0, 0, "Welcome to the PokeCenter!      Press < to leave.");
                        }
                        if(cur_map->t[c->y][c->x].type == TERRAIN_POKEM) {
                            mvprintw(0, 0, "Welcome to the PokeMart!        Press < to leave.");
                        }
                        char menu_input = 0;
                        while(menu_input != '<') {
                            menu_input = getch();
                        }
                    }
                    break;
                    case '5': case '.': case ' ':
                    tookTurn = 1;
                    break;
                    case 't': {
                    int num_trainers = 0;
                    for(int i = 0; i < 21; i++) {
                        for(int j = 0; j < 80; j++) {
                            if(cur_map->characters[i][j] != NULL && cur_map->characters[i][j]->type != PC) {
                                num_trainers++;
                            }
                        }
                    }
                    int tpadx = num_trainers + 2;
                    if(tpadx < 24) tpadx = 24;
                    int tpady = 80;
                    trainerPad = newpad(tpadx, tpady);
                    for(int i = 0; i < 24; i++) {
                        usleep(10000);
                        mvhline(i, 0, ' ', 80);
                    }
                    refresh();
                    mvwprintw(trainerPad, 0, 0, "Trainer List");
                    mvwprintw(trainerPad, 1, 0, "--------------");
                    int placedNPC = 1;
                    for(int i = 0; i < 21; i++) {
                        for(int j = 0; j < 80; j++) {
                            if(cur_map->characters[i][j] != NULL && cur_map->characters[i][j]->type != PC) {
                                placedNPC++;
                                int y_diff = c->y - cur_map->characters[i][j]->y;
                                int x_diff = c->x - cur_map->characters[i][j]->x;
                                const char *y_dir = y_diff >= 0 ? "North" : "South";
                                const char *x_dir = x_diff >= 0 ? "West"  : "East";
                                // found about about the - modifier, pretty awesome, mix it with a string padding and it looks 10x better
                                mvwprintw(trainerPad, placedNPC, 0, " %c  %-35s %2d %s, %2d %s ", cur_map->characters[i][j]->type, cur_map->characters[i][j]->charName, abs(y_diff), y_dir, abs(x_diff), x_dir);
                            }
                        }

                    }

                    int scroll_offset = 0;
                    prefresh(trainerPad, scroll_offset, 0, 0, 0, 23, 79);

                    int menu_input = 0;
                    while(menu_input != 27) {
                        menu_input = getch();
                        if(menu_input == KEY_UP && scroll_offset > 0) {
                            scroll_offset--;
                            prefresh(trainerPad, scroll_offset, 0, 0, 0, 23, 79);
                        }
                        if(menu_input == KEY_DOWN && scroll_offset < tpadx - 24) {
                            scroll_offset++;
                            prefresh(trainerPad, scroll_offset, 0, 0, 0, 23, 79);
                        }
                    }
                    map_print(cur_map);
                    mvhline(0, 0, ' ', 80);
                    mvhline(22, 0, ' ', 80);
                    mvhline(23, 0, ' ', 80);
                    if(debug == 0) {
                        mvprintw(0, 0, "%s", plainMessage);
                    } else {
                        mvprintw(0, 0, "%s", debugMessage);
                    }
                    mvprintw(22, 0, " ");
                    mvprintw(23, 0, "Use y/k/u/l/n/j/b/h to move. Press 'Q' to quit.");
                    refresh();
                    break;
                    }
                    case 'P': {
                        if(debug) {
                            mvprintw(22, 0, "ZA WARUDO!");
                            for(int i = 0; i < 21; i++) {
                                for(int j = 0; j < 80; j++) {
                                    if(cur_map->characters[i][j] != NULL) {
                                        cur_map->characters[i][j]->isDefeated = 1;
                                    }
                                }
                            }
                        }
                    }
                    break;
                    case 'f': {
                        int fly_x = 0;
                        int fly_y = 0;
                        mvhline(0,0, ' ', 80);
                        mvprintw(0,0, "enter X and Y coordinates to fly: ");
                        echo();
                        curs_set(1);
                        char input_buf[80];
                        getnstr(input_buf, 79);
                        int parsed = sscanf(input_buf, "%d %d", &fly_x, &fly_y);
                        if(parsed == 2) {
                            mvprintw(22,0,"x: %d Y:  %d", fly_x, fly_y);
                        }
                        noecho();
                        curs_set(0);
                        if(parsed == 2 && fly_x >= -200 && fly_x <= 200 && fly_y >= -200 && fly_y <= 200) {
                            current_x = fly_x + 200;
                            current_y = fly_y + 200;
                            cur_map->characters[c->y][c->x] = NULL;
                            man_dis = (abs(current_x - 200) + abs(current_y - 200));
                            w.current_y = current_y;
                            w.current_x = current_x;
                            if(w.m[current_x][current_y] == NULL) {
                                map* m = new map();

                                generate_start(TERRAIN_BORDER, m, seeds, &h);
                                generate_roads(m,&w,m->g);
                                generate_builds(m, man_dis);

                                place_npc(m,(rand() % (10 - 2) + 2), &npc);
                                generate_names(m);
                                w.m[current_x][current_y] = m; 
                            }
                                cur_map = w.m[current_x][current_y];
                                int placed = 0;
                            while (!placed) {
                                int rx = rand() % 78 + 1;
                                int ry = rand() % 19 + 1;
                                if (cur_map->t[ry][rx].type == TERRAIN_ROAD && cur_map->t[ry][rx].type != TERRAIN_GATE && cur_map->characters[ry][rx] == NULL) {
                                        c->x = rx;
                                        c->y = ry;
                                        placed = 1;
                                    }
                                }
                                cur_map->characters[c->y][c->x] = c;
                                tookTurn = 1;
                        } else {
                            mvprintw(22,0,"coords out of bounds.");
                        }
                    break;
                    }
                    default:
                    break;
                }
            }

            if(runGame) {
                if (old_map != cur_map) {
                    while (heap_remove_min(&npc)); // Clear the heap to remove old map NPCs
                    int pc_in_map = 0;
                    for (int i = 0; i < 21; i++) {
                        for (int j = 0; j < 80; j++) {
                            if (cur_map->characters[i][j]) {
                                if (cur_map->characters[i][j] != c) {
                                    // Sync NPC time to PC time so they don't take hundreds of consecutive turns
                                    cur_map->characters[i][j]->next_turn = c->next_turn;
                                }
                                heap_insert(&npc, cur_map->characters[i][j]);
                                if (cur_map->characters[i][j] == c) {
                                    pc_in_map = 1;
                                }
                            }
                        }
                    }
                    if (!pc_in_map) heap_insert(&npc, c); // Backup in case the PC wasn't perfectly placed in the arrays yet
                    dijkstra_path(cur_map, HIKER);
                    dijkstra_path(cur_map, RIVAL);
                } else {
                    move_npc(c, cur_map);
                    dijkstra_path(cur_map, HIKER);
                    dijkstra_path(cur_map, RIVAL);
                    heap_insert(&npc, c);
                }
            }
        } else {
            character *battled = move_npc(c, cur_map);
            if (battled) {
                battle_trainer(battled);
                map_print(cur_map);
                mvhline(0, 0, ' ', 80);
                mvhline(22, 0, ' ', 80);
                mvhline(23, 0, ' ', 80);
                if(debug == 0) {
                    mvprintw(0, 0, "%s", plainMessage);
                } else {
                    mvprintw(0, 0, "%s", debugMessage);
                }
                mvprintw(22, 0, " ");
                mvprintw(23, 0, "Use y/k/u/l/n/j/b/h to move. Press 'Q' to quit.");
                refresh();
            }
            heap_insert(&npc, c);
        }
    }
    if(trainerPad) {
        delwin(trainerPad);
    }
    endwin();

    for(int i = 0; i < 401; i++) {
        for(int j = 0; j < 401; j++) {
            if(w.m[i][j] != NULL) {
                delete(w.m[i][j]);
            }
        }
    }
}