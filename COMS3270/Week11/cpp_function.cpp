#include <iostream>

#include "cpp_function.hpp"

void print(const char *) {
    
}

ostream *get_cout()
{
    return &cout;
}

void use_cout(ostream *o, const char *s)
{
    *o << s;
}