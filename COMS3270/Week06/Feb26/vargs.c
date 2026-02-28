#include <stdio.h>
#include <stdarg.h>

//variadic function is a function that can have an undefined amount of parameters, it is shown by ...

/*
This is a simplified printf function it does only 4 conversions: int, char, string, double.
our conversions will take NO modifiers
won't require % signs
wont print anything except the values we convert.
*/
int printf3270(const char *format, ...) {

    va_list va;

    va_start(va, format);

    int i;
    char c;
    float d;
    char *s;

    while(*format) {
        switch(*format) {
            case 'i':
            i = va_arg(va,int);

            printf("int: %d \n", i);
            break;
            case 'c':
            c = va_arg(va, int);
            // the compiler promotes all integer values to int when passing through, so it is important to use int in va_arg when extracting.
            printf("char: %c \n", c);
            break;
            case 'd':
            d = va_arg(va, double);
            printf("double: %f \n", d);
            break;
            case 's':
            s = va_arg(va,char *);
            printf("string: '%s' \n", s);
            break;
            default:
            fprintf(stderr, "Invalid character format: %c \n", *format);
        }
        format++;
    }

    va_end(va);
}

int main(int argc, char * argv) {
    printf3270("i", 42);
    printf3270("s", "hello world", "f", "X");
    printf3270("d", 3.14);
    printf3270("c", 'h');

    return 0;
}