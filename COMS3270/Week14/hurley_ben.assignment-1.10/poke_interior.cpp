#include <stdio.h>
#include <stdlib.h>
#include "poke_logic.hpp"
#include "poke_interior.hpp"
#include <fstream>
#include <iostream>

#define COLUMN 80
#define ROW 21

int generate_caves(map *m, int chance, cave *c)
{
    int space_x, space_y;
    int cave_placed = 0;
    int attempts = 0;

    if ((rand() % 100) < chance && attempts < 200)
    {
        while (!cave_placed)
        {
            attempts++;
            space_x = rand() % (COLUMN - 2) + 1;
            space_y = rand() % (ROW - 2) + 1;
            if (m->t[space_y][space_x].type == TERRAIN_ROCK && (m->t[space_y][space_x + 1].type == TERRAIN_GRASS || m->t[space_y][space_x + 1].type == TERRAIN_CLEAR || m->t[space_y][space_x + 1].type == TERRAIN_ROAD ||
                                                                m->t[space_y][space_x - 1].type == TERRAIN_GRASS || m->t[space_y][space_x - 1].type == TERRAIN_CLEAR || m->t[space_y][space_x - 1].type == TERRAIN_ROAD ||
                                                                m->t[space_y + 1][space_x].type == TERRAIN_GRASS || m->t[space_y + 1][space_x].type == TERRAIN_CLEAR || m->t[space_y + 1][space_x].type == TERRAIN_ROAD ||
                                                                m->t[space_y - 1][space_x].type == TERRAIN_GRASS || m->t[space_y - 1][space_x].type == TERRAIN_CLEAR || m->t[space_y - 1][space_x].type == TERRAIN_ROAD))
            {
                m->t[space_y][space_x].type = TERRAIN_CAVE;
                cave_placed = 1;
            }
        }
    }
    return 0;
}

void enter_pokecenter_interior(map *m, interior *pc)
{

}

void enter_pokemart_interior(map *m, interior *pm)
{
}

void load_pokemart_interior(interior &in, const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not find map file " << filename << std::endl;
        return;
    }

    char c;
    for (int y = 0; y < 14; ++y)
    {
        for (int x = 0; x < 41; ++x)
        {
            // Read a single character, skipping whitespace (newlines)
            if (!(file >> std::noskipws >> c))
                break;

            // If we hit a newline, skip it and get the next actual character
            if (c == '\n' || c == '\r')
            {
                x--; // Don't count the newline as a tile
                continue;
            }

            // Map the character to your tile type
            switch (c)
            {
            case '%':
                in.map[y][x].type = TERRAIN_BORDER;
                break;
            case '#':
                in.map[y][x].type = TERRAIN_ROAD;
                break;
            case '-':
                in.map[y][x].type = TERRAIN_BORDER;
                break; // Counter
            case '[':
            case ']':
            case '_':
                in.map[y][x].type = TERRAIN_GATE;
                break; // Exit/Gates
            default:
                in.map[y][x].type = TERRAIN_CLEAR;
                break;
            }
        }
    }
    file.close();
}

void load_pokecenter_interior(interior &in, const std::string &filename)
{
}