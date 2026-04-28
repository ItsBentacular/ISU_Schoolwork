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
            const char* line3 = is_wild ? "Press ESC to run away." : "Press ESC to exit battle.";
            
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
            mvwprintw(battle_win, 14, (80 - strlen(line3)) / 2, "%s", line3);

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
        if(ch == '\n' || ch == KEY_ENTER || ch == 10) {
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
        
        mvwprintw(npc_box, 1, 2, "%s", p2->base_species->name.c_str());
        mvwprintw(npc_box, 2, 2, "Lv%d", p2->level);
        mvwprintw(npc_box, 3, 2, "HP: %3d / %3d", p2->current_hp, p2->stats[0]);
        
        mvwprintw(pc_box, 1, 2, "%s", p1->base_species->name.c_str());
        mvwprintw(pc_box, 2, 2, "Lv%d", p1->level);
        mvwprintw(pc_box, 3, 2, "HP: %3d / %3d", p1->current_hp, p1->stats[0]);
        
        if (msg) {
            mvwprintw(choice_box, 2, 2, "%s", msg);
            mvwprintw(choice_box, 4, 2, "Press ANY KEY to continue...");
        } else if (menu_state == 0) {
            mvwprintw(choice_box, 1, 2, "What will %s do?", p1->base_species->name.c_str());
            mvwprintw(choice_box, 3, 4, "1. Fight      2. Bag");
            mvwprintw(choice_box, 4, 4, "3. Pokemon    4. Run");
        } else if (menu_state == 1) {
            mvwprintw(choice_box, 1, 2, "Select a move:");
            for (size_t i = 0; i < p1->known_moves.size() && i < 4; i++) {
                mvwprintw(choice_box, 2 + (i/2), 4 + (i%2)*30, "%zu. %s", i+1, p1->known_moves[i]->name.c_str());
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
                if (is_wild) {
                    pc->num_pokeballs--;
                    print_msg("You threw a Pokeball!");
                    if (rand() % 2 == 0) {
                        print_msg("Gotcha! The wild Pokemon was caught!");
                        if (pc->num_pokemon < 6) {
                            pc->pokemon_party[pc->num_pokemon++] = npc->pokemon_party[npc_active];
                            npc->pokemon_party[npc_active] = NULL;
                        }
                        battle_over = 1;
                        break;
                    } else {
                        print_msg("Oh no! The Pokemon broke free!");
                        player_acted = 1;
                    }
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
            
            int xp = npc->pokemon_party[npc_active]->base_species->base_experience * npc->pokemon_party[npc_active]->level;
            sprintf(msg, "%s gained %d XP!", pc->pokemon_party[pc_active]->base_species->name.c_str(), xp);
            print_msg(msg);
            
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
