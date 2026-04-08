#ifndef POKEDATA_H
#define POKEDATA_H

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

template <class P>
struct pokemon {
    int id;
    std::string name;
    int species_id;
    int height;
    int weight;
    int base_experience;
    int order;
    bool is_default;
};

struct moves {
    int id;
    std::string name;
    int gen_id;
    int type_id;
    int power;
    int pp;
    int accuracy;
    int priority;
    int target_id;
    int damage_class_id;
    int effect_id;
    int effect_chance;
    int contest_type_id;
    int contest_effect_id;
    int super_contest_effect_id;
};

struct pokemonMoves {
    int pokemon_id;
    int version_group_id;
    int move_id;
    int pokemon_move_method_id;
    int level;
    int order;
};

struct pokemonSpecies {
    int id;
    std::string name;
    int generation_id;
    int evolves_from_species_id;
    int evolution_chain_id;
    int color_id;
    int shape_id;
    int habitat_id;
    int gender_rate;
    int capture_rate;
    int base_happiness;
    bool is_baby;
    int hatch_counter;
    bool has_gender_differences;
    int growth_rate_id;
    bool forms_switchable;
    bool is_legendary;
    bool is_mythical;
    int order;
    int conquest_order;

};

struct experience {
    int growth_rate_id;
    int level;
    int experience;
};

struct typeNames {
    int type_id;
    // 9 in local_language_id is english, probably just grab english.
    int local_language_id;
    std::string name;
};

struct pokemonStats {
    int pokemon_id;
    int stat_id;
    int base_stat;
    int effort;
};

struct stats {
    int id;
    int damage_class_id;
    std::string identifier;
    bool is_battle_only;
    int game_index;
};

struct pokemonTypes {
    int pokemon_id;
    int type_id;
    int slot;
};

std::string loadPokeData(std::ifstream &file, std::string choice);

#endif