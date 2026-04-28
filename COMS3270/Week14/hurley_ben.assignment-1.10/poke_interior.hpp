#ifndef POKE_INT_H
#define POKE_INT_H

#include <stdio.h>
#include <stdlib.h>
#include "poke_logic.hpp"
#include <fstream>
#include <iostream>

class interior{
public:
    tile map[14][41]= {};
    gate exit[5] = {};
    character* characterMap[14][41]= {};

    interior() = default;
};

class cave{
public:
    tile map[21][40]= {};
    gate exit[1] = {};
    character* characterMap[21][40]= {};

    cave() = default;
};



int generate_caves(map *m, int chance, cave *c);

void enter_pokecenter_interior(map *m, interior *pc);

void enter_pokemart_interior(map *m, interior *pm);

void load_pokemart_interior(interior& in, const std::string& filename);

void load_pokecenter_interior(interior& in, const std::string& filename);

#endif