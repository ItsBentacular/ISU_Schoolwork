#include "string3270.hpp"
#include <cstring>
#include <iostream>

    
    string3270::string3270() { // default, constructs empty string.
        s = strdup("");
    }
    string3270::string3270(const char *str) { // creates a new string with same values as str

    }
    string3270::string3270(string3270 &s) { // copy constructor

    }

    //unlike java, there is NO garbage collection, so every constructor needs a destructor

    // the tilde ~ is required for destructors.
    string3270::~string3270() {
        free(*str);
    }

    // you can use a .operator==() to make comparisons, so really you only need one var for comparison
    bool operator==(string3270 &str){

    }

    bool operator!=(string3270 &str){

    }

    bool operator>=(string3270 &str){

    }

    bool operator<=(string3270 &str){

    }

    bool operator>(string3270 &str){

    }

    bool operator<(string3270 &str){
        return 0;
    }

    // & refers to the original, its saying change the original rather than make a copy.
    string3270 &string3270::operator+=(string3270 &str) {
        char *tmp;

        tmp = (char *)realloc(s, strlen(s) + strlen(str.s) + 1);

        s = tmp;

        strcat(s,str.s);

        return *this;
    }

    string3270 &string3270::operator+=(const char *str) {
        char *tmp;

        tmp = (char *)realloc(s, strlen(s) + strlen(str) + 1);

        s = tmp;

        strcat(s,str);

        return *this;
    }

    string3270 string3270::operator+(string3270 &str) {
        string3270 out(s); // this is the copy constructor, out = s would also work

        out += str;

        return out;
    }

    string3270 operator+(const char *str) {
        string3270 out(s);

        out += str;

        return out;
    }

    string3270 operator=(const string3270 &str) {
        free(s);

        s = strdup(str.s);

        return *this;

    }

    string3270 operator=(const char *str) {
        free(s);

        s = strdup(str);

        return *this;
    }

    int index_of(char c) {
        int i;
        for(i = 0; s[i] && s[i] != c; i++) ;

        return s[i] ? i : -1
    }

    // you can even overload the array syntax if you so choose
    char &string3270::operator[](int idx) {
        return s[idx];
    }

    int length() {
        return strlen(s);
    }

    //this allows us to use these strings in a c function. (Basically a getter)
    const char *string3270::c_str() {
        return s;
    }

    ostream &operator<<(ostream &o, const string3270 &s){
        // return 0 << s.s;wrong, as s is private and ostream/istream cannot access them.
        return 0 << s.c_str();
    }

    // not doing this correctly as it gets very complex, instead shaeffer wants to allocate constant # of bytes and read it, this is a problem if input is larger than alloc. so bug.
    istream &operator>>(istream &i, const string3270 &s){
        
    }
