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
#include <cmath>
#include "poke_interior.hpp"

world w;
std::vector<pokemon> all_pokemon;
std::vector<moves> all_moves;
std::vector<pokemonMoves> all_pokeMoves;
std::vector<pokemonSpecies> all_pokeSpecies;
std::vector<experience> all_experience;
std::vector<typeNames> all_typeNames;
std::vector<pokemonStats> all_pokemonStats;
std::vector<stats> all_stats;
std::vector<pokemonTypes> all_pokemonTypes;

static int calculate_damage(PokemonInstance* attacker, PokemonInstance* defender, const moves* move) {
    if (!move || move->power == INT_MAX || move->power == 0) return 0;
    
    float level = attacker->level;
    float power = move->power;
    float a = attacker->stats[1]; 
    float d = defender->stats[2]; 
    
    float critical = 1.0;
    int base_speed = attacker->stats[5]; 
    if ((rand() % 256) < (base_speed / 2)) {
        critical = 1.5;
    }
    
    float stab = 1.0;
    for (size_t i = 0; i < all_pokemonTypes.size(); i++) {
        if (all_pokemonTypes[i].pokemon_id == attacker->base_species->id && all_pokemonTypes[i].type_id == move->type_id) {
            stab = 1.5;
            break;
        }
    }
    
    float rand_factor = ((rand() % 16) + 85) / 100.0f;
    float type = 1.0; 
    
    int damage = (((((2.0 * level / 5.0) + 2.0) * power * (a / d)) / 50.0) + 2.0) * critical * rand_factor * stab * type;
    if (damage < 1) damage = 1;
    return damage;
}

void battle_handler(character *pc, character *npc, int is_wild, int debug) {
    if (!is_wild && npc->isDefeated) {
        return;
    }

    map *cur_map = w.m[w.current_x][w.current_y];
    WINDOW *battle_win = newwin(24, 80, 0, 0);
    keypad(battle_win, TRUE);

    int in_intro = 1;
    int viewing_stats = 0;
    int current_pokemon = 0;
    int ch;
    
    while(in_intro) {
        if (!viewing_stats) {
            werase(battle_win);
            box(battle_win, 0, 0);
            
            char line1[80];
            
            if (is_wild) {
                PokemonInstance* p = npc->pokemon_party[0];
                sprintf(line1, "A wild %s appeared! (Level %d)", p->base_species->name.c_str(), p->level);
                mvwprintw(battle_win, 10, (80 - strlen(line1)) / 2, "%s", line1);
            } else {
                sprintf(line1, "Battle with %s!", npc->charName);
                mvwprintw(battle_win, 10, (80 - strlen(line1)) / 2, "%s", line1);
            }
            
            if (debug) {
                const char* line4 = is_wild ? "Debug: Press 'v' to view wild stats, ENTER to fight" : "Debug: Press 'v' to view starter stats, ENTER to fight";
                mvwprintw(battle_win, 12, (80 - strlen(line4)) / 2, "%s", line4);
            } else {
                const char* line4 = "Press ENTER to start battle";
                mvwprintw(battle_win, 12, (80 - strlen(line4)) / 2, "%s", line4);
            }

            wrefresh(battle_win);
        } else {
            werase(battle_win);
            box(battle_win, 0, 0);
            
            PokemonInstance* p = npc->pokemon_party[current_pokemon];
            if (is_wild) {
                mvwprintw(battle_win, 2, 4, "A wild %s appeared! (Level %d)", p->base_species->name.c_str(), p->level);
            } else {
                mvwprintw(battle_win, 2, 4, "Pokemon %d of %d: %s (Level %d)", current_pokemon + 1, npc->num_pokemon, p->base_species->name.c_str(), p->level);
            }
            
            mvwprintw(battle_win, 4, 4, "HP:      %3d / %3d", p->current_hp, p->stats[0]);
            mvwprintw(battle_win, 5, 4, "Attack:  %3d", p->stats[1]);
            mvwprintw(battle_win, 6, 4, "Defense: %3d", p->stats[2]);
            mvwprintw(battle_win, 7, 4, "Sp. Atk: %3d", p->stats[3]);
            mvwprintw(battle_win, 8, 4, "Sp. Def: %3d", p->stats[4]);
            mvwprintw(battle_win, 9, 4, "Speed:   %3d", p->stats[5]);
            
            mvwprintw(battle_win, 11, 4, "IVs: HP:%d Atk:%d Def:%d SpA:%d SpD:%d Spe:%d", 
                      p->ivs[0], p->ivs[1], p->ivs[2], p->ivs[3], p->ivs[4], p->ivs[5]);
            mvwprintw(battle_win, 13, 4, "Gender: %s", p->gender == 0 ? "Male" : "Female");
            mvwprintw(battle_win, 14, 4, "Shiny:  %s", p->is_shiny ? "Yes" : "No");
            
            mvwprintw(battle_win, 16, 4, "Known Moves:");
            for (size_t i = 0; i < p->known_moves.size() && i < 4; i++) {
                mvwprintw(battle_win, 17 + i, 6, "- %s", p->known_moves[i]->name.c_str());
            }

            if (!is_wild) {
                const char* line5 = "Use UP / DOWN arrows to scroll party";
                mvwprintw(battle_win, 20, (80 - strlen(line5)) / 2, "%s", line5);
            }
            const char* line6 = "Press 'v' to exit stats view";
            mvwprintw(battle_win, 21, (80 - strlen(line6)) / 2, "%s", line6);
            wrefresh(battle_win);
        }

        ch = wgetch(battle_win);
            if (ch == '\n' || ch == KEY_ENTER || ch == 10) {
            if (!viewing_stats) {
                in_intro = 0;
            }
        } else if ((ch == 'v' || ch == 'V') && debug) {
            viewing_stats = !viewing_stats;
        } else if (viewing_stats) {
            if (ch == KEY_UP && current_pokemon > 0) {
                current_pokemon--;
            } else if (ch == KEY_DOWN && current_pokemon < npc->num_pokemon - 1) {
                current_pokemon++;
            }
        }
    }

    int pc_active = 0;
    int npc_active = 0;
    int attempts = 0;
    int battle_over = 0; // 0=playing, 1=pc win, 2=npc win, 3=fled

    for (int i = 0; i < pc->num_pokemon; i++) {
        if (pc->pokemon_party[i] && pc->pokemon_party[i]->current_hp > 0) {
            pc_active = i;
            break;
        }
    }
    for (int i = 0; i < npc->num_pokemon; i++) {
        if (npc->pokemon_party[i] && npc->pokemon_party[i]->current_hp > 0) {
            npc_active = i;
            break;
        }
    }

    auto draw_battle = [&](const char* msg, int menu_state = 0) {
        werase(battle_win);
        box(battle_win, 0, 0);
        
        WINDOW *pc_box = derwin(battle_win, 5, 27, 10, 48);
        WINDOW *npc_box = derwin(battle_win, 5, 27, 2, 4); 
        WINDOW *choice_box = derwin(battle_win, 7, 78, 16, 1);
        
        box(pc_box, 0, 0);
        box(npc_box, 0, 0);
        box(choice_box, 0, 0);
        
        PokemonInstance* p1 = pc->pokemon_party[pc_active];
        PokemonInstance* p2 = npc->pokemon_party[npc_active];
        
        if (p2) {
            mvwprintw(npc_box, 1, 2, "%s", p2->base_species->name.c_str());
            mvwprintw(npc_box, 2, 2, "Lv%d", p2->level);
            mvwprintw(npc_box, 3, 2, "HP: %3d / %3d", p2->current_hp, p2->stats[0]);
        }
        
        if (p1) {
            mvwprintw(pc_box, 1, 2, "%s", p1->base_species->name.c_str());
            mvwprintw(pc_box, 2, 2, "Lv%d", p1->level);
            mvwprintw(pc_box, 3, 2, "HP: %3d / %3d", p1->current_hp, p1->stats[0]);
        }
        
        if (msg) {
            mvwprintw(choice_box, 2, 2, "%s", msg);
            mvwprintw(choice_box, 4, 2, "Press ANY KEY to continue...");
        } else if (menu_state == 0) {
            if (p1) mvwprintw(choice_box, 1, 2, "What will %s do?", p1->base_species->name.c_str());
            mvwprintw(choice_box, 3, 4, "1. Fight      2. Bag");
            mvwprintw(choice_box, 4, 4, "3. Pokemon    4. Run");
        } else if (menu_state == 1) {
            mvwprintw(choice_box, 1, 2, "Select a move:");
            if (p1) {
                for (size_t i = 0; i < p1->known_moves.size() && i < 4; i++) {
                    mvwprintw(choice_box, 2 + (i/2), 4 + (i%2)*30, "%zu. %s", i+1, p1->known_moves[i]->name.c_str());
                }
            }
            mvwprintw(choice_box, 5, 2, "0. Cancel");
        } else if (menu_state == 2) {
            mvwprintw(choice_box, 1, 2, "Select an item:");
            mvwprintw(choice_box, 2, 4, "1. Potion (x%d)   2. Revive (x%d)", pc->num_potions, pc->num_revives);
            mvwprintw(choice_box, 3, 4, "3. Pokeball (x%d)", pc->num_pokeballs);
            mvwprintw(choice_box, 5, 2, "0. Cancel");
        } else if (menu_state == 3) {
            mvwprintw(choice_box, 1, 2, "Select a Pokemon:");
            for (int i = 0; i < pc->num_pokemon; i++) {
                if (pc->pokemon_party[i]) {
                    mvwprintw(choice_box, 2 + (i/2), 4 + (i%2)*35, "%d. %s (%d/%d)", 
                        i+1, pc->pokemon_party[i]->base_species->name.c_str(), 
                        pc->pokemon_party[i]->current_hp, pc->pokemon_party[i]->stats[0]);
                }
            }
            mvwprintw(choice_box, 5, 2, "0. Cancel");
        }
        
        wrefresh(battle_win);
        wrefresh(pc_box);
        wrefresh(npc_box);
        wrefresh(choice_box);
        
        delwin(pc_box);
        delwin(npc_box);
        delwin(choice_box);
    };

    auto print_msg = [&](const char* msg) {
        draw_battle(msg, 0);
        wgetch(battle_win);
    };

    while (battle_over == 0) {
        draw_battle(NULL, 0);
        
        ch = wgetch(battle_win);
        int player_acted = 0;
        
        if (ch == '1') {
            draw_battle(NULL, 1);
            int m_ch = wgetch(battle_win);
            if (m_ch >= '1' && m_ch <= '4') {
                size_t move_idx = m_ch - '1';
                PokemonInstance* active_p = pc->pokemon_party[pc_active];
                if (move_idx < active_p->known_moves.size()) {
                    const moves* sel_move = active_p->known_moves[move_idx];
                    int hit_chance = sel_move->accuracy != INT_MAX ? sel_move->accuracy : 100;
                    char msg[100];
                    if ((rand() % 100) < hit_chance) {
                        int dmg = calculate_damage(active_p, npc->pokemon_party[npc_active], sel_move);
                        npc->pokemon_party[npc_active]->current_hp -= dmg;
                        if (npc->pokemon_party[npc_active]->current_hp < 0) 
                            npc->pokemon_party[npc_active]->current_hp = 0;
                        sprintf(msg, "%s used %s! Dealt %d damage.", active_p->base_species->name.c_str(), sel_move->name.c_str(), dmg);
                    } else {
                        sprintf(msg, "%s used %s! But it missed!", active_p->base_species->name.c_str(), sel_move->name.c_str());
                    }
                    print_msg(msg);
                    player_acted = 1;
                }
            }
        } else if (ch == '2') {
            draw_battle(NULL, 2);
            int i_ch = wgetch(battle_win);
            if (i_ch == '1' && pc->num_potions > 0) {
                pc->pokemon_party[pc_active]->current_hp += 20; 
                if (pc->pokemon_party[pc_active]->current_hp > pc->pokemon_party[pc_active]->stats[0]) 
                    pc->pokemon_party[pc_active]->current_hp = pc->pokemon_party[pc_active]->stats[0];
                pc->num_potions--;
                print_msg("You used a Potion! HP restored.");
                player_acted = 1;
            } else if (i_ch == '2' && pc->num_revives > 0) {
                int revived = 0;
                for(int i = 0; i < pc->num_pokemon; i++) {
                    if(pc->pokemon_party[i] && pc->pokemon_party[i]->current_hp <= 0) {
                        pc->pokemon_party[i]->current_hp = pc->pokemon_party[i]->stats[0] / 2;
                        pc->num_revives--;
                        print_msg("You used a Revive!");
                        revived = 1;
                        player_acted = 1;
                        break;
                    }
                }
                if(!revived) print_msg("No Pokemon needs reviving.");
            } else if (i_ch == '3' && pc->num_pokeballs > 0) {
                if (is_wild && pc->num_pokemon < 6) {
                    pc->num_pokeballs--;
                    print_msg("You threw a Pokeball!");
                    if (rand() % 2 == 0) {
                        print_msg("Gotcha! The wild Pokemon was caught!");
                        if (pc->num_pokemon < 6) {
                            PokemonInstance *poke_catch = new PokemonInstance();
                            poke_catch = npc->pokemon_party[npc_active];
                            pc->pokemon_party[pc->num_pokemon++] = poke_catch;
                            npc->pokemon_party[npc_active] = NULL;
                        }
                        battle_over = 1;
                        break;
                    } else {
                        print_msg("Oh no! The Pokemon broke free!");
                        player_acted = 1;
                    }
                } else if(pc->num_pokemon == 6 && is_wild) {
                    print_msg("You already have 6 pokemon!");
                } else {
                print_msg("You can't catch a trainer's Pokemon you dolt!");
                }
            }
        } else if (ch == '3') {
            draw_battle(NULL, 3);
            int p_ch = wgetch(battle_win);
            if (p_ch >= '1' && p_ch < '1' + pc->num_pokemon) {
                int p_idx = p_ch - '1';
                if (p_idx == pc_active) {
                    print_msg("That Pokemon is already in battle!");
                } else if (pc->pokemon_party[p_idx]->current_hp <= 0) {
                    print_msg("That Pokemon has no energy left!");
                } else {
                    pc_active = p_idx;
                    char msg[100];
                    sprintf(msg, "Go! %s!", pc->pokemon_party[pc_active]->base_species->name.c_str());
                    print_msg(msg);
                    player_acted = 1;
                }
            }
        } else if (ch == '4') {
            if (!is_wild) {
                print_msg("Don't be a coward and run from a Trainer battle!");
            } else {
                attempts++;
                int wild_speed = npc->pokemon_party[npc_active]->stats[5];
                int my_speed = pc->pokemon_party[pc_active]->stats[5];
                int b = (wild_speed / 4) % 256;
                int escape = 0;
                if (b == 0) escape = 1;
                else {
                    int f = (my_speed * 32) / b + 30 * attempts;
                    if ((rand() % 256) < f) escape = 1;
                }
                if (escape) {
                    print_msg("Got away safely!");
                    battle_over = 3;
                    break;
                } else {
                    print_msg("Can't escape!");
                    player_acted = 1;
                }
            }
        }
        
        if (battle_over != 0) break;

        if (npc->pokemon_party[npc_active]->current_hp <= 0) {
            char msg[100];
            sprintf(msg, "Foe %s fainted!", npc->pokemon_party[npc_active]->base_species->name.c_str());
            print_msg(msg);
            
            int xp = (npc->pokemon_party[npc_active]->base_species->base_experience * npc->pokemon_party[npc_active]->level) / 7 ;
            sprintf(msg, "%s gained %d XP!", pc->pokemon_party[pc_active]->base_species->name.c_str(), xp);
            print_msg(msg);
            
            PokemonInstance* active_p = pc->pokemon_party[pc_active];
            active_p->current_xp += xp;

            // Check for level ups using the experience curve
            bool leveled_up = false;
            while (active_p->level < 100) {
                int next_level_xp = INT_MAX;
                for (const auto& exp : all_experience) {
                    if (exp.growth_rate_id == active_p->growth_rate_id && exp.level == active_p->level + 1) {
                        next_level_xp = exp.experience;
                        break;
                    }
                }
                
                if (active_p->current_xp >= next_level_xp) {
                    active_p->level++;
                    leveled_up = true;
                    sprintf(msg, "%s grew to level %d!", active_p->base_species->name.c_str(), active_p->level);
                    print_msg(msg);
                } else {
                    break; // Hasn't reached the next level's XP requirement
                }
            }

            if (leveled_up) {
                // Recalculate stats for the new level
                int base_stats[6] = {0, 0, 0, 0, 0, 0};
                for (const auto& ps : all_pokemonStats) {
                    if (ps.pokemon_id == active_p->base_species->id && ps.stat_id >= 1 && ps.stat_id <= 6) {
                        base_stats[ps.stat_id - 1] = ps.base_stat;
                    }
                }
                int old_hp_max = active_p->stats[0];
                for (int i = 0; i < 6; i++) {
                    active_p->stats[i] = (((base_stats[i] + active_p->ivs[i]) * 2) * active_p->level) / 100 + (i == 0 ? active_p->level + 10 : 5);
                }
                // Heal the Pokemon by the amount its max HP increased
                active_p->current_hp += (active_p->stats[0] - old_hp_max);
            }

            int next_npc = -1;
            for (int i = 0; i < npc->num_pokemon; i++) {
                if (npc->pokemon_party[i] && npc->pokemon_party[i]->current_hp > 0) {
                    next_npc = i;
                    break;
                }
            }
            if (next_npc == -1) {
                battle_over = 1;
                break;
            } else {
                npc_active = next_npc;
                sprintf(msg, "%s sent out %s!", npc->charName ? npc->charName : "Trainer", npc->pokemon_party[npc_active]->base_species->name.c_str());
                print_msg(msg);
            }
        }

        if (player_acted && battle_over == 0) {
            int npc_acted = 0;
            int action = rand() % 100;
            
            int needs_revive = -1;
            int needs_potion = -1;
            int min_hp_pct = 100;
            for (int i = 0; i < npc->num_pokemon; i++) {
                if (!npc->pokemon_party[i]) continue;
                if (npc->pokemon_party[i]->current_hp <= 0) needs_revive = i;
                else {
                    int pct = (npc->pokemon_party[i]->current_hp * 100) / npc->pokemon_party[i]->stats[0];
                    if (pct < 50 && pct < min_hp_pct) {
                        min_hp_pct = pct;
                        needs_potion = i;
                    }
                }
            }
            
            if (action < 20 && (needs_revive != -1 || needs_potion != -1) && (npc->num_potions > 0 || npc->num_revives > 0)) {
                char msg[100];
                if (needs_revive != -1 && npc->num_revives > 0) {
                    npc->pokemon_party[needs_revive]->current_hp = npc->pokemon_party[needs_revive]->stats[0] / 2;
                    npc->num_revives--;
                    sprintf(msg, "%s used a Revive!", npc->charName ? npc->charName : "Trainer");
                    print_msg(msg);
                    npc_acted = 1;
                } else if (needs_potion != -1 && npc->num_potions > 0) {
                    npc->pokemon_party[needs_potion]->current_hp += 20;
                    if (npc->pokemon_party[needs_potion]->current_hp > npc->pokemon_party[needs_potion]->stats[0])
                        npc->pokemon_party[needs_potion]->current_hp = npc->pokemon_party[needs_potion]->stats[0];
                    npc->num_potions--;
                    sprintf(msg, "%s used a Potion!", npc->charName ? npc->charName : "Trainer");
                    print_msg(msg);
                    npc_acted = 1;
                }
            } else if (action >= 20 && action < 30 && npc->num_pokemon > 1) {
                int choices[6];
                int count = 0;
                for (int i = 0; i < npc->num_pokemon; i++) {
                    if (i != npc_active && npc->pokemon_party[i] && npc->pokemon_party[i]->current_hp > 0) {
                        choices[count++] = i;
                    }
                }
                if (count > 0) {
                    npc_active = choices[rand() % count];
                    char msg[100];
                    sprintf(msg, "%s switched to %s!", npc->charName ? npc->charName : "Trainer", npc->pokemon_party[npc_active]->base_species->name.c_str());
                    print_msg(msg);
                    npc_acted = 1;
                }
            }
            
            if (!npc_acted) {
                PokemonInstance* n_active = npc->pokemon_party[npc_active];
                if (n_active->known_moves.size() > 0) {
                    const moves* sel_move = n_active->known_moves[rand() % n_active->known_moves.size()];
                    int hit_chance = sel_move->accuracy != INT_MAX ? sel_move->accuracy : 100;
                    char msg[100];
                    if ((rand() % 100) < hit_chance) {
                        int dmg = calculate_damage(n_active, pc->pokemon_party[pc_active], sel_move);
                        pc->pokemon_party[pc_active]->current_hp -= dmg;
                        if (pc->pokemon_party[pc_active]->current_hp < 0) 
                            pc->pokemon_party[pc_active]->current_hp = 0;
                        sprintf(msg, "Foe %s used %s! Dealt %d damage.", n_active->base_species->name.c_str(), sel_move->name.c_str(), dmg);
                    } else {
                        sprintf(msg, "Foe %s used %s! But it missed!", n_active->base_species->name.c_str(), sel_move->name.c_str());
                    }
                    print_msg(msg);
                }
            }
            
            if (pc->pokemon_party[pc_active]->current_hp <= 0) {
                char msg[100];
                sprintf(msg, "%s fainted!", pc->pokemon_party[pc_active]->base_species->name.c_str());
                print_msg(msg);
                
                int next_pc = -1;
                for (int i = 0; i < pc->num_pokemon; i++) {
                    if (pc->pokemon_party[i] && pc->pokemon_party[i]->current_hp > 0) {
                        next_pc = i;
                        break;
                    }
                }
                if (next_pc == -1) {
                    battle_over = 2; 
                } else {
                    draw_battle(NULL, 3);
                    int switched = 0;
                    while (!switched) {
                        int p_ch = wgetch(battle_win);
                        if (p_ch >= '1' && p_ch < '1' + pc->num_pokemon) {
                            int p_idx = p_ch - '1';
                            if (pc->pokemon_party[p_idx]->current_hp > 0) {
                                pc_active = p_idx;
                                sprintf(msg, "Go! %s!", pc->pokemon_party[pc_active]->base_species->name.c_str());
                                print_msg(msg);
                                switched = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    if (battle_over == 1) { 
        print_msg("You won the battle!");
        if (!is_wild) npc->isDefeated = 1;
    } else if (battle_over == 2) { 
        print_msg("You blacked out!");
        for (int i = 0; i < pc->num_pokemon; i++) {
            if (pc->pokemon_party[i]) {
                pc->pokemon_party[i]->current_hp = pc->pokemon_party[i]->stats[0];
            }
        }
        if (cur_map) {
            int min_dist = INT_MAX;
            int px = pc->x, py = pc->y;
            for(int i = 1; i < 20; i++) {
                for(int j = 1; j < 79; j++) {
                    if(cur_map->t[i][j].type == TERRAIN_POKEC) {
                        int dist = abs(pc->x - j) + abs(pc->y - i);
                        if(dist < min_dist && cur_map->characters[i][j] == NULL) {
                            min_dist = dist;
                            px = j;
                            py = i;
                        }
                    }
                }
            }
            if (min_dist != INT_MAX) {
                cur_map->characters[pc->y][pc->x] = NULL;
                pc->x = px;
                pc->y = py;
                cur_map->characters[py][px] = pc;
            }
        }
    }

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


void refill_items(character *c) {
    if(c->num_pokeballs < 5) {
        c->num_pokeballs = 5;
    }
    if(c->num_potions < 6 ) {
        c->num_potions = 6;
    }
    if(c->num_revives < 3) {
        c->num_revives = 3;
    }
}

void heal_pokemon(character *c) {
    for(int i = 0; i < c->num_pokemon; i++) {
        c->pokemon_party[i]->current_hp = c->pokemon_party[i]->stats[0];
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    // Load all database files needed for the game to function
    const char* db_files[] = {
        "pokemon", "moves", "pokemon_moves", "pokemon_species",
        "experience", "type_names", "pokemon_stats", "stats", "pokemon_types"
    };
    for (int i = 0; i < 9; i++) {
        std::ifstream f;
        if (open_poke_file(f, db_files[i])) {
            loadPokeData(f, db_files[i]);
        }
    }
    if (all_pokemon.empty()) {
        std::cerr << "Error: Pokemon database failed to load. Please ensure the CSV files are present." << std::endl;
        return 1;
    }

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

    interior *pc = new interior();
    interior *pm = new interior();
    cave *gen_cave = new cave();

    if(w.m[current_x][current_y] == NULL) {
        map * m = new map();

        heap_init(&npc, character_cmp, NULL);

        generate_start(TERRAIN_BORDER, m, seeds, &h);
        generate_roads(m,&w,m->g);
        generate_builds(m, man_dis);
        generate_caves(m, 90, gen_cave);
        place_pc(m, &npc);

        for(int i = 1; i < argc; i++) {
            if(strcmp(argv[i], "--numtrainers") == 0) {
                if(i + 1 < argc) {
                    place_npc(m, atoi(argv[i + 1]), &npc, man_dis, all_pokemon, all_pokeMoves, all_moves, all_pokemonStats);
                    i++;   
                } else {
                    printf("--numtrainers requires a integer value after.\n");
                    return -1;
                }
        } else if(strcmp(argv[i], "--debug") == 0) {
            debug = 1;
        } else {
            // assignment 1.07 case is here, if it is a database file name print it and exit
            printPokeData(argv[i]);
                return 0;
            }
        }
        if(argc < 2 || debug) {
            place_npc(m,(rand() % (10 - 2) + 2), &npc, man_dis, all_pokemon, all_pokeMoves, all_moves, all_pokemonStats);
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

        int choiceMade = 0;
        character *new_char = new character(0,0,SENTRY,0,0,0,0);
        new_char->num_pokemon = 3;
        for(int i = 0; i < 3; i++) {
            new_char->pokemon_party[i] = generate_pokemon(man_dis, all_pokemon, all_pokeMoves, all_moves, all_pokemonStats);
        }
        
        refresh(); // Draw standard screen background so the choice menu doesn't black-screen
        PokemonInstance *player_poke = NULL;
        while(!choiceMade) {
            WINDOW *choice_win = newwin(24, 80, 0, 0);
            box(choice_win, 0, 0);
        
            char line1[80];
            char line2[80];
            char line3[80];
            const char* line4 = "Choose your starter with 1, 2, or 3";
            char line5[80];
            
            sprintf(line1, "Welcome to the world of POKEMON!");
            sprintf(line2, "before I let you go, I need you to pick your first pokemon:");
            sprintf(line3, "1: %s, 2: %s, 3: %s", new_char->pokemon_party[0]->base_species->name.c_str(), new_char->pokemon_party[1]->base_species->name.c_str(),new_char->pokemon_party[2]->base_species->name.c_str());
            
            mvwprintw(choice_win, 9, (80 - strlen(line1)) / 2, "%s", line1);
            mvwprintw(choice_win, 10, (80 - strlen(line2)) / 2, "%s", line2);
            mvwprintw(choice_win, 11, (80 - strlen(line3)) / 2, "%s", line3);
            mvwprintw(choice_win, 12, (80 - strlen(line4)) / 2, "%s", line4);

            wrefresh(choice_win);
            int ch = getch();
            if(ch == '1' || ch == '2' || ch == '3') {
                int choice_idx = ch - '1';
                player_poke = new_char->pokemon_party[choice_idx];
                choiceMade = 1;
                new_char->pokemon_party[choice_idx] = NULL; // Avoid double-free
                sprintf(line5, "You chose: %s", player_poke->base_species->name.c_str());
                mvwprintw(choice_win, 13, (80 - strlen(line5)) / 2, "%s", line5);
                wrefresh(choice_win);
                usleep(1500000); // Wait 1.5 seconds so you can see the text
            }
            delwin(choice_win); // Prevent memory leaks if loop repeats
        }
        
        clear(); // Wipe the menu off the screen completely
        refresh();

        // Find PC and assign the chosen starter Pokemon
        character *player_character = NULL;
        for (int i = 0; i < 21; i++) {
            for (int j = 0; j < 80; j++) {
                if (cur_map->characters[i][j] && cur_map->characters[i][j]->type == PC) {
                    player_character = cur_map->characters[i][j];
                    break;
                }
            }
            if (player_character) break;
        }

        if (player_character && player_poke) {
            player_character->pokemon_party[0] = player_poke;
            player_character->num_pokemon = 1;
        }

        delete new_char;


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
                int old_x = c->x;
                int old_y = c->y;
                int input = getch();

                switch(input) {
                    case 'Q':
                    runGame = 0;
                    break;
                    case 'y': case '7':
                    if(move_cost(c->type, cur_map->t[c->y - 1][c->x - 1].type) != INT_MAX) {
                        if(cur_map->characters[c->y - 1][c->x - 1] != NULL && !cur_map->characters[c->y - 1][c->x - 1]->isDefeated) {
                            battle_handler(c, cur_map->characters[c->y - 1][c->x - 1], 0, debug);
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
                            generate_caves(m, 20, gen_cave);

                            place_npc(m,(rand() % (10 - 2) + 2), &npc, man_dis, all_pokemon, all_pokeMoves, all_moves, all_pokemonStats);
                            generate_names(m);
                            w.m[current_x][current_y] = m;
                            
                        }
                        cur_map = w.m[current_x][current_y];
                        c->y += 19;
                        cur_map->characters[c->y][c->x] = c;

                    }
                    if(move_cost(c->type, cur_map->t[c->y - 1][c->x].type) != INT_MAX) {
                        if(cur_map->characters[c->y - 1][c->x] != NULL && !cur_map->characters[c->y - 1][c->x]->isDefeated) {
                            battle_handler(c, cur_map->characters[c->y - 1][c->x], 0, debug);
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
                            battle_handler(c, cur_map->characters[c->y - 1][c->x + 1], 0, debug);
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
                            generate_caves(m, 20, gen_cave);

                            place_npc(m,(rand() % (10 - 2) + 2), &npc, man_dis, all_pokemon, all_pokeMoves, all_moves, all_pokemonStats);
                            generate_names(m);
                            w.m[current_x][current_y] = m;
                            
                        }
                        cur_map = w.m[current_x][current_y];
                        c->x = 1;
                        cur_map->characters[c->y][c->x] = c;

                    }
                    if(move_cost(c->type, cur_map->t[c->y][c->x + 1].type) != INT_MAX) {
                        if(cur_map->characters[c->y][c->x + 1] != NULL && !cur_map->characters[c->y][c->x + 1]->isDefeated) {
                            battle_handler(c, cur_map->characters[c->y][c->x + 1], 0, debug);
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
                            battle_handler(c, cur_map->characters[c->y + 1][c->x + 1], 0, debug);
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
                            generate_caves(m, 20, gen_cave);

                            place_npc(m,(rand() % (10 - 2) + 2), &npc, man_dis, all_pokemon, all_pokeMoves, all_moves, all_pokemonStats);
                            generate_names(m);
                            w.m[current_x][current_y] = m;
                            
                        }
                        cur_map = w.m[current_x][current_y];
                        c->y -= 19;
                        cur_map->characters[c->y][c->x] = c;

                    }
                    if(move_cost(c->type, cur_map->t[c->y + 1][c->x].type) != INT_MAX) {
                        if(cur_map->characters[c->y + 1][c->x] != NULL && !cur_map->characters[c->y + 1][c->x]->isDefeated) {
                            battle_handler(c, cur_map->characters[c->y + 1][c->x], 0, debug);
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
                            battle_handler(c, cur_map->characters[c->y + 1][c->x - 1], 0, debug);
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
                            generate_caves(m, 20, gen_cave);

                            place_npc(m,(rand() % (10 - 2) + 2), &npc, man_dis, all_pokemon, all_pokeMoves, all_moves, all_pokemonStats);
                            generate_names(m);
                            w.m[current_x][current_y] = m;
                            
                        }
                        cur_map = w.m[current_x][current_y];
                        c->x = 78;
                        cur_map->characters[c->y][c->x] = c;
                    }
                    if(move_cost(c->type, cur_map->t[c->y][c->x - 1].type) != INT_MAX) {
                        if(cur_map->characters[c->y][c->x - 1] != NULL && !cur_map->characters[c->y][c->x - 1]->isDefeated) {
                            battle_handler(c, cur_map->characters[c->y][c->x - 1], 0, debug);
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
                    if(cur_map->t[c->y][c->x].type == TERRAIN_POKEC || cur_map->t[c->y][c->x].type == TERRAIN_POKEM || cur_map->t[c->y][c->x].type == TERRAIN_CAVE) {
                        mvhline(0, 0, ' ', 80);
                        if(cur_map->t[c->y][c->x].type == TERRAIN_POKEC) {
                            mvprintw(0, 0, "Welcome to the PokeCenter! We've healed all of your pokemon! |Press < to leave.|");
                            enter_pokecenter_interior(cur_map, pc);
                            heal_pokemon(c);

                        }
                        if(cur_map->t[c->y][c->x].type == TERRAIN_POKEM) {
                            mvprintw(0, 0, "Welcome to the PokeMart! We've refilled all of your items  |Press < to leave.|");
                            enter_pokemart_interior(cur_map, pm);
                            refill_items(c);
                        }
                        if(cur_map->t[c->y][c->x].type == TERRAIN_CAVE) {
                            mvprintw(0, 0, "Oh no! A scary cave, you better leave with \"<\"!");
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
                                generate_caves(m, 2, gen_cave);

                                place_npc(m,(rand() % (10 - 2) + 2), &npc, man_dis, all_pokemon, all_pokeMoves, all_moves, all_pokemonStats);
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

                if (tookTurn && (c->x != old_x || c->y != old_y)) {
                    if (cur_map->t[c->y][c->x].type == TERRAIN_GRASS) {
                        if ((rand() % 100) < 10) {
                            character *wild_npc = new character(0, 0, SENTRY, 0, 0, 0, 0);
                            wild_npc->num_pokemon = 1;
                            wild_npc->pokemon_party[0] = generate_pokemon(man_dis, all_pokemon, all_pokeMoves, all_moves, all_pokemonStats);
                            battle_handler(c, wild_npc, 1, debug);
                            delete wild_npc;
                        }
                    }
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
                battle_handler(battled, c, 0, debug); // 'battled' is now PC, 'c' is NPC
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