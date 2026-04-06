#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

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
    const std::string &path1 = "/share/cs327/pokedex/pokedex/data/csv/" + filename[fileChoice];
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
            if (!file.is_open())
            {
                file.clear();
                if (!path3.empty())
                {
                    file.open(path3);
                    if (!file.is_open())
                    {
                        return "CRASH";
                    }
                }
            }
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::cout << buffer.str() << "NEXT FILE" << std::endl;
        file.close();
    }
    return "SUCCESS";
}

int main(int argc, char *argv[])
{
    std::ifstream file;
    std::cout << loadPokeData(file, argv[1]) << std::endl;
}