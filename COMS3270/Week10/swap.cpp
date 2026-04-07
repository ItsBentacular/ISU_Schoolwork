#include <iostream>

using namespace std;

//Generics are called Templates in C++.
// you can template both methods and classes.

void swap(int &a, int &b) {
    int tmp;

    tmp = a;
    a = b;
    b = tmp;
    //for each variable type you would need a swap function, but templates save the day...
}

template <class T>
void swapTemplate(T &a, T &b) {
    T tmp;

    tmp = a;
    a = b;
    b = tmp;
}

class foo {
    public:
    int i;
    foo() {i = 1;}

};

int main(int argc, char *argv[]) {
    string i, j;
    i = "true";
    j = "false";
    // this can be called multiple times and will generate a function for each variable type, does not have erasure like java, so it using templates multiple times will take up more space.
    swapTemplate(i , j);

    cout << "i = " << i << ", j = " << j << endl;

    foo b,c;
    c.i = 2;

    swapTemplate(b,c);

    cout << "b = " << b.i << ", c = " << c.i << endl;
    return 0;
}