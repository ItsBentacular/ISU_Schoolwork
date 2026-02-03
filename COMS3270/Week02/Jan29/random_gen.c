#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    // you only use this ONCE at the start of your function, using it multiple times will not make it "more random".
    srand(time(NULL));

    printf("%d\n", rand());
    printf("%d\n", rand());
    printf("%d\n", rand());
    printf("%d\n", rand());
    printf("%d\n", rand());
}
