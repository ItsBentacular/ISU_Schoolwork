#include "cpp_function.hpp"

void print(const char *);

int main(int argc, char* argv[]) {
    ostream *o;

    print("Hello World!\n");

    o = get_cout();
    use_cout(o, "Goodbye!\n");

    return 0;
}