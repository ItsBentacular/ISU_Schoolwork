#include <iostream>

using namespace std;

// when we inherit from A multiple times, A essentially becomes an interface
// A may have instance variables; however, we can only use the default contructor
class A {
public:
    A(){  cout << __PRETTY_FUNCTION__ << endl; }
    A(int i){
    cout << __PRETTY_FUNCTION__ << endl; 
    }
    A(A &a){ 
    cout << __PRETTY_FUNCTION__ << endl; 
    }

    virtual ~A() { cout << __PRETTY_FUNCTION__ << endl; }

    void print() { cout << __PRETTY_FUNCTION__ << endl; }
};

class B : virtual public A {
public:
    // classes need to reference the classes they extend from when initializing values, see A(i), i(i) as proof.
    B(){ cout << __PRETTY_FUNCTION__ << endl; }
    B(B &b){ cout << __PRETTY_FUNCTION__ << endl; }
    virtual ~B() { cout << __PRETTY_FUNCTION__ << endl; }

    void print() { cout << __PRETTY_FUNCTION__ << endl; }
};

class C : virtual public A {
public:
    C(){ cout << __PRETTY_FUNCTION__ << endl; }
    C(C &c){ cout << __PRETTY_FUNCTION__ << endl; }
    virtual ~C() { cout << __PRETTY_FUNCTION__ << endl; }

    void print() { cout << __PRETTY_FUNCTION__ << endl; }
};

class D : virtual public B , virtual public C {
private:
int i;
public:
    D(){ cout << __PRETTY_FUNCTION__ << endl; }
    D(D &d){ cout << __PRETTY_FUNCTION__ << endl; }
    virtual ~D() { cout << __PRETTY_FUNCTION__ << endl; }

    void print() { cout << __PRETTY_FUNCTION__ << endl; }
};

int main(int argc, char *argv[]) {
    // automatic/static instance: A a; vs. dynamic instance: A *a = new A;
    // D obj;
    // you can also specifically use the scope resolution to specify which class you want to use rather than cast. ((B) obj).print is cast vs. obj.B::print is scope.
    //((B) obj).A::print(); is the only way to reach A through D without being ambigous in C++
    //obj.A::print();
    A *ap;
    ap = new D;
    delete ap;

    return 0;
}