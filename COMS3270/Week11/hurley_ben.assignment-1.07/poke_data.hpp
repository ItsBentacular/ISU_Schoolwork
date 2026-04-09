#ifndef POKEDATA_H
#define POKEDATA_H

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

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

extern std::vector<pokemon> all_pokemon;
extern std::vector<moves> all_moves;
extern std::vector<pokemonMoves> all_pokeMoves;
extern std::vector<pokemonSpecies> all_pokeSpecies;
extern std::vector<experience> all_experience;
extern std::vector<typeNames> all_typeNames;
extern std::vector<pokemonStats> all_pokemonStats;
extern std::vector<stats> all_stats;
extern std::vector<pokemonTypes> all_pokemonTypes;

bool open_poke_file(std::ifstream &file, const std::string& filename);
std::string loadPokeData(std::ifstream &file, std::string choice);
void load_pokemon(std::ifstream &file, std::vector<pokemon> *vec);
void load_moves(std::ifstream &file, std::vector<moves> *vec);
void load_pokemon_moves(std::ifstream &file, std::vector<pokemonMoves> *vec);
void load_pokemon_species(std::ifstream &file, std::vector<pokemonSpecies> *vec);
void load_experience(std::ifstream &file, std::vector<experience> *vec);
void load_type_names(std::ifstream &file, std::vector<typeNames> *vec);
void load_pokemon_stats(std::ifstream &file, std::vector<pokemonStats> *vec);
void load_stats(std::ifstream &file, std::vector<stats> *vec);
void load_pokemon_types(std::ifstream &file, std::vector<pokemonTypes> *vec);
void printPokeData(const std::string& choice);

#endif