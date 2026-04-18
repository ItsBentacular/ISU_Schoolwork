//singleton method is when one creates a class wherein it is only possible to create (and use) one instance.
#include <iostream>

using namespace std;

class singleton {
private:
    singleton(int i) : i(i) { cout << __PRETTY_FUNCTION__ << endl; }
    static singleton *single_instance; // this is a definition, nothing is actually created yet; no space is allocated, see singleton outside of class below.

public:
    int i;
    static singleton *get_instance() {
        if(!single_instance) {
            return (single_instance = new singleton(9999));
        }
        return single_instance;
    }
    ostream &print(ostream &o) const {
        return o << this << ", " << i;
    }





};

ostream &operator<<(ostream &o, const singleton &s) {
    return s.print(o);
}

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

    // time to break some code!! singleton is supposed to be a single instantiation class, but with C malloc, we can break it!

    cout << *s << endl;

    singleton *t;

    // is this cool? Yes. Should you do this? definitely not, Prof. just wanted to show us that you can get around C++ constraints by using C due to the fact that C++ is built on C.
    t = (singleton *) malloc(sizeof (singleton));
    t->i = 3;
   cout << *t << endl;

    return 0;
}

