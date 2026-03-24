#include <stdio.h>

int main(int argc, char * argv[]) {
    //printf("You're Plastic\n"); q(a)


    /* q(b)
    int i;
    char *a[] = {"happen ", "to ", "make ", "stop ", "\"fetch\" ", "trying ",};
    int o[] = {3,5,1,2,4,0};

    for(i = 0; i < 6; i++) {
        printf("%s", a[o[i]]);
    }
    // stop trying to make "fetch" happen
    */
//            0           1             2    
//            01 2 3456789012 3 45 6 7890 1 2345 6
char s [] = "On Wednesdays we wear pink!";
*( s + 1) = 'O';
*( s + 2) = 'n';
*( s + 3) = ' ';
*( s + 4) = 'S';
*( s + 5) = 'a';
*( s + 6) = 't';
*( s + 7) = 'u';
*( s + 8) = 'r';
*( s + 26) = '.';
printf ("%s\n" , s + 1);

//On 
//
}  