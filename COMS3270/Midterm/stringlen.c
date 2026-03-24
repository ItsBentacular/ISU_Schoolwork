#include <stdio.h>



int getstrlen(char * string) {
    int i;
    for(i = 0; string[i] != '\0'; i++);
    return i;
}

int main(int argc, char * argv[]) {
    char *s = "I love COMS 3270";

    int length = getstrlen(s);

    printf("int val is: %d\n", length);

    char * str = "I LOVE BEES";
    printf("%s\n", str);
    str = "I HATE BEES";
    printf("%s\n", str);
}