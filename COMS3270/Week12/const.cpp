#include <stdio.h>

int main(int argc, char * argv[]) {
    const volatile int i = 0;
    // while const signals that this variable is not to be changed, volatile tells the compiler that there may be changes to the value outside of the control of this code.
    // This is usually used in multithreading code or in code that directly interfaces with hardware. This specific usage is broken/a mistake! DO NOT DO THIS

    printf("%d\n", i);

    *((int *) &i) = 1;

    printf("%d\n", i);

    return 0;
}