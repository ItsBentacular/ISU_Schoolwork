#ifndef STRING3270_H
#define STRING3270_H

#include <iostream>
// in a valid instance of a class, s points to a valid string. if we have an instance, it points to a valid string, if if we have an empty instance, it points to the empty string.
//s is malloc
class string3270 {
    private:
        char *s;

    public:
        string3270(); // default, constructs empty string.
            
        string3270(const char *str); // creates a new string with same values as str

        string3270(string3270 &s); // copy constructor

        //unlike java, there is NO garbage collection, so every constructor needs a destructor

        // the tilde ~ is required for destructors.
        ~string3270();

        // you can use a .operator==() to make comparisons, so really you only need one var for comparison
        bool operator==(string3270 &str);
        bool operator!=(string3270 &str);
        bool operator>=(string3270 &str);
        bool operator<=(string3270 &str);
        bool operator>(string3270 &str);
        bool operator<(string3270 &str);

        // & refers to the original, its saying change the original rather than make a copy.
        string3270 &operator+=(string3270 &str);
        string3270 &operator+=(const char *str);
        string3270 operator+(string3270 &str);
        string3270 operator+(const char *str);
        string3270 &operator=(string3270 &str);
        string3270 &operator=(const char *str);

        int index_of(char c);

        // you can even overload the array syntax if you so choose
        char &operator[](int idx);
        int length();
        //this allows us to use these strings in a c function.
        const char *c_str();


};

    ostream &operator<<(ostream &o, const string3270 &s);

    istream &operator>>(istream &i, const string3270 &s);

    
#endif