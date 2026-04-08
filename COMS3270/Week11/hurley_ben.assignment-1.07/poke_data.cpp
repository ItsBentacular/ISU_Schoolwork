#include "poke_data.hpp"
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <climits>

std::vector<pokemon> all_pokemon;
std::vector<moves> all_moves;
std::vector<pokemonMoves> all_pokeMoves;
std::vector<pokemonSpecies> all_pokeSpecies;
// std::vector<moves> all_moves;
// ... and so on for other data types


// maybe do a generic here and add a generic param?? that way you could pass as reference and store data into main (ex instead of pokemon p, moves m, you could just have pokedex T, and let compiler figure it out.)
std::string loadPokeData(std::ifstream &file, std::string choice)
{
    const std::vector<std::string> filename = {"pokemon.csv", "moves.csv", "pokemon_moves.csv", "pokemon_species.csv", "experience.csv", "type_names.csv", "pokemon_stats.csv", "stats.csv", "pokemon_types.csv"};
    const char *homeDirectory = getenv("HOME");
    int fileChoice = -1; // Good practice to initialize it
    //I feel like switch cases should be able to handle strings but whatever ig
    if (choice == "pokemon")
    {
        fileChoice = 0;
    }
    else if (choice == "moves")
    {
        fileChoice = 1;
    }
    else if (choice == "pokemon_moves")
    {
        fileChoice = 2;
    }
    else if (choice == "pokemon_species")
    {
        fileChoice = 3;
    }
    else if (choice == "experience")
    {
        fileChoice = 4;
    }
    else if (choice == "type_names")
    {
        fileChoice = 5;
    }
    else if (choice == "pokemon_stats")
    {
        fileChoice = 6;
    }
    else if (choice == "stats")
    {
        fileChoice = 7;
    }
    else if (choice == "pokemon_types")
    {
        fileChoice = 8;
    }
    else
    {
        std::cerr << "Invalid database choice." << std::endl;
        return "CRASH";
    }
    const std::string path1 = "/share/cs327/pokedex/pokedex/data/csv/" + filename[fileChoice];
    std::string path2 = "";
    if (homeDirectory != nullptr)
    {
        path2 = std::string(homeDirectory) + "/.poke327/pokedex/pokedex/data/csv/" + filename[fileChoice];
    }
    const std::string &path3 = "../../../../pokedex/pokedex/data/csv/" + filename[fileChoice];
    // C:\ISU\pokedex\pokedex\data\csv
    //  /share/cs327/pokedex/data/csv

    file.open(path1);
    if (!file.is_open())
    {
        file.clear();
        if (!path2.empty())
        {
            file.open(path2);
        }
    }
    if (!file.is_open())
    {
        file.clear();
        if (!path3.empty())
        {
            file.open(path3);
        }
    }

    if (!file.is_open())
    {
        std::cerr << "Could not open any of the pokedex files for " << filename[fileChoice] << std::endl;
        return "CRASH";
    }

    // --- PARSING LOGIC ---
    std::string header;
    std::getline(file, header); // Read and discard the header line.

    //pokemon
    if (fileChoice == 0)
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            std::stringstream ss(line);
            std::string field;
            pokemon p;

            // id
            std::getline(ss, field, ',');
            p.id = field.empty() ? INT_MAX : std::stoi(field);

            // name (identifier)
            std::getline(ss, field, ',');
            p.name = field;

            // species_id
            std::getline(ss, field, ',');
            p.species_id = field.empty() ? INT_MAX : std::stoi(field);

            // height
            std::getline(ss, field, ',');
            p.height = field.empty() ? INT_MAX : std::stoi(field);

            // weight
            std::getline(ss, field, ',');
            p.weight = field.empty() ? INT_MAX : std::stoi(field);

            // base_experience
            std::getline(ss, field, ',');
            p.base_experience = field.empty() ? INT_MAX : std::stoi(field);

            // order
            std::getline(ss, field, ',');
            p.order = field.empty() ? INT_MAX : std::stoi(field);

            // is_default
            std::getline(ss, field, ',');
            p.is_default = (field == "1");

            all_pokemon.push_back(p);
        }
    }
    // moves
    if (fileChoice == 1)
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            std::stringstream ss(line);
            std::string field;
            moves m;

            // id
            std::getline(ss, field, ',');
            m.id = field.empty() ? INT_MAX : std::stoi(field);

            // name
            std::getline(ss,field, ',');
            m.name = field;

            //gen_id
            std::getline(ss, field,',' );
            m.gen_id = field.empty() ? INT_MAX : std::stoi(field);

            //type_id
            std::getline(ss,field,',');
            m.type_id = field.empty() ? INT_MAX : std::stoi(field);

            //power
            std::getline(ss,field, ',');
            m.power = field.empty() ? INT_MAX : std::stoi(field);

            //pp
            std::getline(ss, field, ',');
            m.pp = field.empty() ? INT_MAX : std::stoi(field);

            //accuracy
            std::getline(ss,field, ',');
            m.accuracy = field.empty() ? INT_MAX : std::stoi(field);

            //priority
            std::getline(ss, field, ',');
            m.priority = field.empty() ? INT_MAX : std::stoi(field);

            //target_id
            std::getline(ss,field, ',');
            m.target_id = field.empty() ? INT_MAX : std::stoi(field);

            //damage_class_id
            std::getline(ss,field, ',');
            m.damage_class_id = field.empty() ? INT_MAX : std::stoi(field);
    
            //effect_id
            std::getline(ss,field, ',');
            m.effect_id = field.empty() ? INT_MAX : std::stoi(field);

            //effect_chance
            std::getline(ss,field, ',');
            m.effect_chance = field.empty() ? INT_MAX : std::stoi(field);

            //contest_type_id
            std::getline(ss,field, ',');
            m.contest_type_id = field.empty() ? INT_MAX : std::stoi(field);

            //contest_effect_id
            std::getline(ss,field, ',');
            m.contest_effect_id = field.empty() ? INT_MAX : std::stoi(field);

            //super_contest_effect_id
            std::getline(ss,field, ',');
            m.super_contest_effect_id = field.empty() ? INT_MAX : std::stoi(field);

            all_moves.push_back(m);
        }
    }
    // pokemon_moves
    if (fileChoice == 2)
    {
    std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            std::stringstream ss(line);
            std::string field;
            pokemonMoves pm;

            //pokemon_id
            std::getline(ss, field,',');
            pm.pokemon_id = field.empty() ? INT_MAX : std::stoi(field);

            //version_group_id
            std::getlines(ss, field, ',');
            pm.version_group_id = field.empty() ? INT_MAX : std::atoi(field);
            
            //move_id
            std::getlines(ss, field,',');
            pm.move_id = field.empty() ? INT_MAX : std::atoi(field);

            //pokemon_move_method_id
            std::getlines(ss, field,',');
            pm.pokemon_move_method_id = field.empty() ? INT_MAX : std::atoi(field);

            //level
            std::getlines(ss,field,',');
            pm.level = field.empty() ? INT_MAX : std::atoi(field);

            //order
            std::getlines(ss,field,',');
            pm.order = field.empty() ? INT_MAX : std::atoi(field);
        }
    }
    //Pokemon Species
    if (fileChoice == 3)
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            std::stringstream ss(line);
            std::string field;
            pokemonSpecies ps;

            // id
            std::getline(ss, field, ',');
            ps.id = field.empty() ? INT_MAX : std::stoi(field);

            // identifier
            std::getline(ss, field, ',');
            ps.identifier = field;

            // generation_id
            std::getline(ss, field, ',');
            ps.generation_id = field.empty() ? INT_MAX : std::stoi(field);

            // evolves_from_species_id
            std::getline(ss, field, ',');
            ps.evolves_from_species_id = field.empty() ? INT_MAX : std::stoi(field);

            // evolution_chain_id
            std::getline(ss, field, ',');
            ps.evolution_chain_id = field.empty() ? INT_MAX : std::stoi(field);

            // color_id
            std::getline(ss, field, ',');
            ps.color_id = field.empty() ? INT_MAX : std::stoi(field);

            // shape_id
            std::getline(ss, field, ',');
            ps.shape_id = field.empty() ? INT_MAX : std::stoi(field);

            // habitat_id
            std::getline(ss, field, ',');
            ps.habitat_id = field.empty() ? INT_MAX : std::stoi(field);

            // gender_rate
            std::getline(ss, field, ',');
            ps.gender_rate = field.empty() ? INT_MAX : std::stoi(field);

            // capture_rate
            std::getline(ss, field, ',');
            ps.capture_rate = field.empty() ? INT_MAX : std::stoi(field);

            // base_happiness
            std::getline(ss, field, ',');
            ps.base_happiness = field.empty() ? INT_MAX : std::stoi(field);   
                 
            // is_baby
            std::getline(ss, field, ',');
            ps.is_baby = (field == "1");

            // hatch_counter
            std::getline(ss, field, ',');
            ps.hatch_counter = field.empty() ? INT_MAX : std::stoi(field);

            // has_gender_differences
            std::getline(ss, field, ',');
            ps.has_gender_differences = (field == "1");

            // growth_rate_id
            std::getline(ss, field, ',');
            ps.growth_rate_id = field.empty() ? INT_MAX : std::stoi(field);

            // forms_switchable
            std::getline(ss, field, ',');
            ps.forms_switchable = (field == "1");

            // order
            std::getline(ss, field, ',');
            ps.order = field.empty() ? INT_MAX : std::stoi(field);

            // conquest_order
            std::getline(ss, field, ',');
            ps.conquest_order = field.empty() ? INT_MAX : std::stoi(field);

            // Note: You would typically have a vector for species like:
            // all_species.push_back(ps);
        

    }
    if (fileChoice == 4)
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            std::stringstream ss(line);
            std::string field;
            pokemon p;

            // id
            std::getline(ss, field, ',');
            p.id = field.empty() ? -1 : std::stoi(field);

            // name (identifier)
            std::getline(ss, field, ',');
            p.name = field;

            // species_id
            std::getline(ss, field, ',');
            p.species_id = field.empty() ? -1 : std::stoi(field);

            // height
            std::getline(ss, field, ',');
            p.height = field.empty() ? -1 : std::stoi(field);

            // weight
            std::getline(ss, field, ',');
            p.weight = field.empty() ? -1 : std::stoi(field);

            // base_experience
            std::getline(ss, field, ',');
            p.base_experience = field.empty() ? -1 : std::stoi(field);

            // order
            std::getline(ss, field, ',');
            p.order = field.empty() ? -1 : std::stoi(field);

            // is_default
            std::getline(ss, field, ',');
            p.is_default = (field == "1");

            all_pokemon.push_back(p);
        }
    }
    if (fileChoice == 5)
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            std::stringstream ss(line);
            std::string field;
            pokemon p;

            // id
            std::getline(ss, field, ',');
            p.id = field.empty() ? -1 : std::stoi(field);

            // name (identifier)
            std::getline(ss, field, ',');
            p.name = field;

            // species_id
            std::getline(ss, field, ',');
            p.species_id = field.empty() ? -1 : std::stoi(field);

            // height
            std::getline(ss, field, ',');
            p.height = field.empty() ? -1 : std::stoi(field);

            // weight
            std::getline(ss, field, ',');
            p.weight = field.empty() ? -1 : std::stoi(field);

            // base_experience
            std::getline(ss, field, ',');
            p.base_experience = field.empty() ? -1 : std::stoi(field);

            // order
            std::getline(ss, field, ',');
            p.order = field.empty() ? -1 : std::stoi(field);

            // is_default
            std::getline(ss, field, ',');
            p.is_default = (field == "1");

            all_pokemon.push_back(p);
        }
    }
    if (fileChoice == 6)
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            std::stringstream ss(line);
            std::string field;
            pokemon p;

            // id
            std::getline(ss, field, ',');
            p.id = field.empty() ? -1 : std::stoi(field);

            // name (identifier)
            std::getline(ss, field, ',');
            p.name = field;

            // species_id
            std::getline(ss, field, ',');
            p.species_id = field.empty() ? -1 : std::stoi(field);

            // height
            std::getline(ss, field, ',');
            p.height = field.empty() ? -1 : std::stoi(field);

            // weight
            std::getline(ss, field, ',');
            p.weight = field.empty() ? -1 : std::stoi(field);

            // base_experience
            std::getline(ss, field, ',');
            p.base_experience = field.empty() ? -1 : std::stoi(field);

            // order
            std::getline(ss, field, ',');
            p.order = field.empty() ? -1 : std::stoi(field);

            // is_default
            std::getline(ss, field, ',');
            p.is_default = (field == "1");

            all_pokemon.push_back(p);
        }
    }
    if (fileChoice == 7)
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            std::stringstream ss(line);
            std::string field;
            pokemon p;

            // id
            std::getline(ss, field, ',');
            p.id = field.empty() ? -1 : std::stoi(field);

            // name (identifier)
            std::getline(ss, field, ',');
            p.name = field;

            // species_id
            std::getline(ss, field, ',');
            p.species_id = field.empty() ? -1 : std::stoi(field);

            // height
            std::getline(ss, field, ',');
            p.height = field.empty() ? -1 : std::stoi(field);

            // weight
            std::getline(ss, field, ',');
            p.weight = field.empty() ? -1 : std::stoi(field);

            // base_experience
            std::getline(ss, field, ',');
            p.base_experience = field.empty() ? -1 : std::stoi(field);

            // order
            std::getline(ss, field, ',');
            p.order = field.empty() ? -1 : std::stoi(field);

            // is_default
            std::getline(ss, field, ',');
            p.is_default = (field == "1");

            all_pokemon.push_back(p);
        }
    }
    if (fileChoice == 8)
    {
        std::string line;
        while (std::getline(file, line))
        {
            
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            std::stringstream ss(line);
            std::string field;
            pokemon p;

            // id
            std::getline(ss, field, ',');
            p.id = field.empty() ? -1 : std::stoi(field);

            // name (identifier)
            std::getline(ss, field, ',');
            p.name = field;

            // species_id
            std::getline(ss, field, ',');
            p.species_id = field.empty() ? -1 : std::stoi(field);

            // height
            std::getline(ss, field, ',');
            p.height = field.empty() ? -1 : std::stoi(field);

            // weight
            std::getline(ss, field, ',');
            p.weight = field.empty() ? -1 : std::stoi(field);

            // base_experience
            std::getline(ss, field, ',');
            p.base_experience = field.empty() ? -1 : std::stoi(field);

            // order
            std::getline(ss, field, ',');
            p.order = field.empty() ? -1 : std::stoi(field);

            // is_default
            std::getline(ss, field, ',');
            p.is_default = (field == "1");

            all_pokemon.push_back(p);
        }
    }
        /* 
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::cout << buffer.str() << std::endl;
        file.close();
        */

    file.close();
    return "SUCCESS";
}

int main(int argc, char *argv[])
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
                              << ", Name: " << all_pokemon[i].name
                              << ", Height: " << all_pokemon[i].height << std::endl;
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