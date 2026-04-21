#include "poke_data.hpp"
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <climits>

bool open_poke_file(std::ifstream &file, const std::string& filename)
{
    const char *homeDirectory = getenv("HOME");
    const std::string path1 = "/share/cs327/pokedex/pokedex/data/csv/" + filename + ".csv";
    std::string path2 = "";
    if (homeDirectory != nullptr)
    {
        path2 = std::string(homeDirectory) + "/.poke327/pokedex/pokedex/data/csv/" + filename + ".csv";
    }
    const std::string path3 = "../../../../pokedex/pokedex/data/csv/" + filename + ".csv";

    file.open(path1);
    if (!file.is_open())
    {
        file.clear();
        if (!path2.empty())
            file.open(path2);
    }
    if (!file.is_open())
    {
        file.clear();
        if (!path3.empty())
            file.open(path3);
    }

    if (!file.is_open())
    {
        std::cerr << "Could not open any of the pokedex files for " << filename << std::endl;
        return false;
    }
    return true;
}

void load_pokemon(std::ifstream &file, std::vector<pokemon> *vec) {
    std::string header;
    std::getline(file, header); 
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::stringstream ss(line);
        std::string field;
        pokemon p;
        std::getline(ss, field, ','); p.id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); p.name = field;
        std::getline(ss, field, ','); p.species_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); p.height = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); p.weight = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); p.base_experience = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); p.order = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); p.is_default = (field == "1");
        vec->push_back(p);
    }
}

void load_moves(std::ifstream &file, std::vector<moves> *vec) {
    std::string header;
    std::getline(file, header); 
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::stringstream ss(line);
        std::string field;
        moves m;
        std::getline(ss, field, ','); m.id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.name = field;
        std::getline(ss, field, ','); m.gen_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.type_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.power = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.pp = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.accuracy = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.priority = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.target_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.damage_class_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.effect_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.effect_chance = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.contest_type_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.contest_effect_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); m.super_contest_effect_id = field.empty() ? INT_MAX : std::stoi(field);
        vec->push_back(m);
    }
}

void load_pokemon_moves(std::ifstream &file, std::vector<pokemonMoves> *vec) {
    std::string header;
    std::getline(file, header);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::stringstream ss(line);
        std::string field;
        pokemonMoves pm;
        std::getline(ss, field, ','); pm.pokemon_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); pm.version_group_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); pm.move_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); pm.pokemon_move_method_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); pm.level = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); pm.order = field.empty() ? INT_MAX : std::stoi(field);
        vec->push_back(pm);
    }
}

void load_pokemon_species(std::ifstream &file, std::vector<pokemonSpecies> *vec) {
    std::string header;
    std::getline(file, header); 
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::stringstream ss(line);
        std::string field;
        pokemonSpecies ps;
        std::getline(ss, field, ','); ps.id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.name = field;
        std::getline(ss, field, ','); ps.generation_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.evolves_from_species_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.evolution_chain_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.color_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.shape_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.habitat_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.gender_rate = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.capture_rate = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.base_happiness = field.empty() ? INT_MAX : std::stoi(field);   
        std::getline(ss, field, ','); ps.is_baby = (field == "1");
        std::getline(ss, field, ','); ps.hatch_counter = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.has_gender_differences = (field == "1");
        std::getline(ss, field, ','); ps.growth_rate_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.forms_switchable = (field == "1");
        std::getline(ss, field, ','); ps.is_legendary = (field == "1");
        std::getline(ss, field, ','); ps.is_mythical = (field == "1");
        std::getline(ss, field, ','); ps.order = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.conquest_order = field.empty() ? INT_MAX : std::stoi(field);
        vec->push_back(ps);
    }
}

void load_experience(std::ifstream &file, std::vector<experience> *vec) {
    std::string header;
    std::getline(file, header);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::stringstream ss(line);
        std::string field;
        experience e;
        std::getline(ss, field, ','); e.growth_rate_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); e.level = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); e.experience = field.empty() ? INT_MAX : std::stoi(field);
        vec->push_back(e);
    }
}

void load_type_names(std::ifstream &file, std::vector<typeNames> *vec) {
    std::string header;
    std::getline(file, header);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::stringstream ss(line);
        std::string field;
        typeNames t;
        std::getline(ss, field, ','); t.type_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); t.local_language_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); t.name = field;
        vec->push_back(t);
    }
}

void load_pokemon_stats(std::ifstream &file, std::vector<pokemonStats> *vec) {
    std::string header;
    std::getline(file, header);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::stringstream ss(line);
        std::string field;
        pokemonStats ps;
        std::getline(ss, field, ','); ps.pokemon_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.stat_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.base_stat = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); ps.effort = field.empty() ? INT_MAX : std::stoi(field);
        vec->push_back(ps);
    }
}

void load_stats(std::ifstream &file, std::vector<stats> *vec) {
    std::string header;
    std::getline(file, header);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::stringstream ss(line);
        std::string field;
        stats s;
        std::getline(ss, field, ','); s.id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); s.damage_class_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); s.identifier = field;
        std::getline(ss, field, ','); s.is_battle_only = (field == "1");
        std::getline(ss, field, ','); s.game_index = field.empty() ? INT_MAX : std::stoi(field);
        vec->push_back(s);
    }
}

void load_pokemon_types(std::ifstream &file, std::vector<pokemonTypes> *vec) {
    std::string header;
    std::getline(file, header);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::stringstream ss(line);
        std::string field;
        pokemonTypes pt;
        std::getline(ss, field, ','); pt.pokemon_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); pt.type_id = field.empty() ? INT_MAX : std::stoi(field);
        std::getline(ss, field, ','); pt.slot = field.empty() ? INT_MAX : std::stoi(field);
        vec->push_back(pt);
    }
}

std::string loadPokeData(std::ifstream &file, std::string choice)
{
    if (choice == "pokemon") {
        load_pokemon(file, &all_pokemon);
    } else if (choice == "moves") {
        load_moves(file, &all_moves);
    } else if (choice == "pokemon_moves") {
        load_pokemon_moves(file, &all_pokeMoves);
    } else if (choice == "pokemon_species") {
        load_pokemon_species(file, &all_pokeSpecies);
    } else if (choice == "experience") {
        load_experience(file, &all_experience);
    } else if (choice == "type_names") {
        load_type_names(file, &all_typeNames);
    } else if (choice == "pokemon_stats") {
        load_pokemon_stats(file, &all_pokemonStats);
    } else if (choice == "stats") {
        load_stats(file, &all_stats);
    } else if (choice == "pokemon_types") {
        load_pokemon_types(file, &all_pokemonTypes);
    } else {
        std::cerr << "Invalid database choice." << std::endl;
        return "CRASH";
    }

    file.close();
    return "SUCCESS";
}

void printPokeData(const std::string& choice) {
    auto print_int = [](int val) { return val == INT_MAX ? "" : std::to_string(val); };
    std::ostringstream buffer;

    if (choice == "pokemon") {
        for (const auto &p : all_pokemon) {
            buffer << print_int(p.id) << "," << p.name << "," << print_int(p.species_id) << ","
                   << print_int(p.height) << "," << print_int(p.weight) << "," << print_int(p.base_experience) << ","
                   << print_int(p.order) << "," << p.is_default << "\n";
        }
    } else if (choice == "moves") {
        for (const auto &m : all_moves) {
            buffer << print_int(m.id) << "," << m.name << "," << print_int(m.gen_id) << ","
                   << print_int(m.type_id) << "," << print_int(m.power) << "," << print_int(m.pp) << ","
                   << print_int(m.accuracy) << "," << print_int(m.priority) << "," << print_int(m.target_id) << ","
                   << print_int(m.damage_class_id) << "," << print_int(m.effect_id) << ","
                   << print_int(m.effect_chance) << "," << print_int(m.contest_type_id) << ","
                   << print_int(m.contest_effect_id) << "," << print_int(m.super_contest_effect_id) << "\n";
        }
    } else if (choice == "pokemon_moves") {
        for (const auto &pm : all_pokeMoves) {
            buffer << print_int(pm.pokemon_id) << "," << print_int(pm.version_group_id) << ","
                   << print_int(pm.move_id) << "," << print_int(pm.pokemon_move_method_id) << ","
                   << print_int(pm.level) << "," << print_int(pm.order) << "\n";
        }
    } else if (choice == "pokemon_species") {
        for (const auto &ps : all_pokeSpecies) {
            buffer << print_int(ps.id) << "," << ps.name << "," << print_int(ps.generation_id) << ","
                   << print_int(ps.evolves_from_species_id) << "," << print_int(ps.evolution_chain_id) << ","
                   << print_int(ps.color_id) << "," << print_int(ps.shape_id) << "," << print_int(ps.habitat_id) << ","
                   << print_int(ps.gender_rate) << "," << print_int(ps.capture_rate) << ","
                   << print_int(ps.base_happiness) << "," << ps.is_baby << "," << print_int(ps.hatch_counter) << ","
                   << ps.has_gender_differences << "," << print_int(ps.growth_rate_id) << "," << ps.forms_switchable << ","
                   << ps.is_legendary << "," << ps.is_mythical << "," << print_int(ps.order) << "," << print_int(ps.conquest_order) << "\n";
        }
    } else if (choice == "experience") {
        for (const auto &e : all_experience) {
            buffer << print_int(e.growth_rate_id) << "," << print_int(e.level) << "," << print_int(e.experience) << "\n";
        }
    } else if (choice == "type_names") {
        for (const auto &tn : all_typeNames) {
            if(tn.local_language_id == 9){
                buffer << print_int(tn.type_id) << "," << print_int(tn.local_language_id) << "," << tn.name << "\n";
            }
        }
    } else if (choice == "pokemon_stats") {
        for (const auto &ps : all_pokemonStats) {
            buffer << print_int(ps.pokemon_id) << "," << print_int(ps.stat_id) << "," << print_int(ps.base_stat) << "," << print_int(ps.effort) << "\n";
        }
    } else if (choice == "stats") {
        for (const auto &s : all_stats) {
            buffer << print_int(s.id) << "," << print_int(s.damage_class_id) << "," << s.identifier << "," << s.is_battle_only << "," << print_int(s.game_index) << "\n";
        }
    } else if (choice == "pokemon_types") {
        for (const auto &pt : all_pokemonTypes) {
            buffer << print_int(pt.pokemon_id) << "," << print_int(pt.type_id) << "," << print_int(pt.slot) << "\n";
        }
    }

    // Dump the entire buffer out
    std::cout << buffer.str();
}

/*int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <data_type>" << std::endl;
        std::cerr << "e.g., " << argv[0] << " pokemon" << std::endl;
        return 1;
    }

    std::ifstream file;
    std::string result = loadPokeData(file, argv[1]);

    if (result == "SUCCESS")
    {
        std::cout << "Successfully loaded and parsed data for: " << argv[1] << std::endl;

        if (std::string(argv[1]) == "moves")
        {
            std::cout << "Total pokemon loaded: " << all_moves.size() << std::endl;
            if (!all_moves.empty()) {
                std::cout << "First 10 Pokemon:" << std::endl;
                for (size_t i = 0; i < 10 && i < all_moves.size(); ++i)
                {
                    std::cout << "  ID: " << all_moves[i].id
                              << ", Name: " << all_moves[i].name
                              << ", Power: " << all_moves[i].power << std::endl;
                }
            }
        }
        // Add other 'if' cases here to test other data types
    }
    else
    {
        std::cerr << "Failed to load data." << std::endl;
    }

    return 0;
}
*/