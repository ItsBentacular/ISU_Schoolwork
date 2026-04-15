//singleton method is when one creates a class wherein it is only possible to create (and use) one instance.
#include <iostream>

using namespace std;

class singleton {
private:
    singleton() { cout << __PRETTY_FUNCTION__ << endl; }
    static singleton *single_instance; // this is a definition, nothing is actually created yet; no space is allocated, see singleton outside of class below.

public:
    static singleton *get_instance() {
        if(!single_instance) {
            return (single_instance = new singleton());
        }
        return single_instance;
    }





};

//instatiating the static variable defined in singleton private, basically the private class defines it, but this line actually makes it exist.
// without this, we would get a link-time error.
singleton *singleton::single_instance;

int main(int argc, char * argv[]) {
    singleton *s = singleton::get_instance();
    singleton::get_instance();
    singleton::get_instance();
    singleton::get_instance();
    singleton::get_instance();
    singleton::get_instance();
    singleton::get_instance();

    return 0;
}

