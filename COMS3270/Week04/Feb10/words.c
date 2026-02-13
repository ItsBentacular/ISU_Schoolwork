#include <stdio.h>
#include <ctype.h>

char *words[] = {
    "alphabet",
    "bongos",
    "computer",
    "dog",
    "end",
    "flagrant",
    "gazebo",
    "hat",
    "iguana",
    "jolly",
    "kitty",
    "lung",
    "mother",
    "nitrogen",
    "otters",
    "plinth",
    "queen",
    "rotisserie",
    "supercilious",
    "tomato",
    "ultraviolet",
    "ventriliquism",
    "wretch",
    "xylophone",
    "you",
    "zapdos",
};

int main(int argc, char *argv[]) {

    printf("%c is for %s!\n", toupper(argv[1][0]), words[tolower(argv[1][0]) - 'a']);


    return 0;
}