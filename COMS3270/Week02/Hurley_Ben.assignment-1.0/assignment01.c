#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROW 21
#define COLUMN 80

int map[ROW][COLUMN];

int main(int argc, char *argv[]) {
    srand(time(NULL));
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COLUMN; j++) {
            printf(":");
        }
        printf("\n");
    }


}