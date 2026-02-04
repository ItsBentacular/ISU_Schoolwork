#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//strings in C language are character arrays that are terminated by a null byte at the end.
// the null byte is "\0", a sentinel, which is a marker in an array to stop once it is found.
// all string funcitons in the stdlib depend on this byte, if it is missing, then the array is not a string.
//functions on character arrays without a null terminator.
// When declaring a string literal, we can use either two syntaxes:
// 1: char s[] = "Foo!"; this is modifiable, as it lives on the stack or data segment
// 2: char *p = "Bar!"; this is NOT modifiable, as it lives in the text segment, but can be pointed somewhere else.
// both of these are valid strings
//usually you do not need to declare the size with strings

size_t strlen327(const char *s) {
    //This works, but no experienced C programmer would do it like this (apparently)
    // sizeof(s) DOES NOT WORK, it will return the pointer memory, not the length of the string!

    size_t i;
    // this works, because as soon as it finds a NULL byte it becomes false, as NULL is false.
    for(i = 0; s[i]; i++);

    return i;
}

int strcmp327(const char *s1, const char *s2) {
    while((*s1 == *s2) && *s1) {
        s1++;
        s2++;

    }
    return *s1 - *s2;
}

char *strcpy327(char *dest, const char *src) {
    int i;
    for(i = 0; src[i]; i++) {
        dest[i] = src[i];
    }

     dest[i] = '\0'; 

    return dest;
}
// you could also replace the loop with for(i = 0; dest[i] = src[i]; i++); which both adds the values and checks the truth value at the same time.

int main(int argc, char *argv[]) {

    char *p = "hello world!";
    char *t;

    printf("strlen327: %lu, strlen: %lu\n", strlen327(p), strlen(p));

    printf("%s\n", p);

    t = "Goodbye";
    printf("s: '%s' , t: '%s', strcmp327(s, t): %d, strcmp(s, t): %d\n", p, t, strcmp327(p,t), strcmp(p,t));
    t = "Goodbye!";
    printf("s: '%s' , t: '%s', strcmp327(s, t): %d, strcmp(s, t): %d\n", p, t, strcmp327(p,t), strcmp(p,t));

    // the code below will crash, as t is immutable space (pointer).
    //strcpy327(t,s)

    // plus 1 because of null byte
    t = malloc(sizeof (*t) * (strlen327(p) + 1)); // sizeof is still used just for good programming, while t may be one, multiplying it allows for the variable t to be changed without needing any different byte allocation
    // now t points to allocated space on the heap.
    strcpy327(t, p);
    printf("t after: %s\n", t);

    // free all allocated memory.
    free(t);

    return 0;

}