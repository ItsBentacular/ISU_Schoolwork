#include "string3270.hpp"

    
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

    bool operator>=(string3270 &str);

    bool operator<=(string3270 &str);

    bool operator>(string3270 &str);

    bool operator<(string3270 &str);

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

    string3270 operator+(const char *str);

    string3270 operator=(string3270 &str);

    string3270 operator=(const char *str);

    int index_of(char c);

    // you can even overload the array syntax if you so choose
    char &operator[](int idx);
    int length();
    //this allows us to use these strings in a c function.
    const char *c_str();