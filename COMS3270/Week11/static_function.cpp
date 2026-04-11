#include <iostream>
#include "static_function.hpp"

using namespace std;

static void static_func()
{
    cout << __PRETTY_FUNCTION__ << endl;
}

void use_static_func()
{
    static_func();
}