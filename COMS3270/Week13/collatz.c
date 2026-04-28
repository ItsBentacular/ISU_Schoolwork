#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int n = atoi(argv[1]);

    while (n != 1) {
        printf("n: %d \n", n);
        if(!(n % 2)) {
            n /= 2;
        } else {
            n = n * 3 + 1;
        }
    }

}