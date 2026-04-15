#include <iostream>
#include <exception>

using namespace std;

class borked {
public:
    void foo() {
        throw "bar failed";
    }
};

class exception3270 : public std::exception {
private:
    const char *s;
public:
    exception3270(const char *s) : s(s) {}
    const char  *what() {
        return s;
    }


};

class alsoborked {
public:
    void bar() {
        // you can technically throw any type you want in c++, even your own classes!
        throw "foo failed";
    }
};

int main (int argc, char *argv[]) {
    borked b;
    try{
    b.foo();
    } catch(int i) {
        cout << i << endl;
    } catch(const char *s) {
        cout << s << endl;\
    // this must always be the last catch statement, as it grabs all types of exceptions (kinda a last resort)
    } catch(...) {
        cout << "caught some exception" << endl;
    }
    return 0;
}