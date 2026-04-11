#include <iostream>
#include "static_function.hpp"

using namespace std;

class static_example {
public:
    static int foo;

    static void static_method()
    {
        foo++;
    }
};

int static_example::foo = 1;

int count_calls()
{
    // static means that variable is kept alive for entirety of program, meaning that the function always keeps the variable updated, never resets.

    return ++static_example::foo;
}

int main(int argc, char * argv[]) 
{
    int i;

    for(i = 0; i < 10; i++) {
       cout << count_calls() << endl;
    }

    use_static_func();
}