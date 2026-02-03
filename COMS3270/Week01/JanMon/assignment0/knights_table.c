#include <stdio.h>
#include <stdlib.h>

struct coords {
    int x; // row
    int y; // column
}

int main(int argc, char *argv[])
{
    int M,N;
    if(argc > 1) {
        M = atoi(argv[1]);
        N = atoi(argv[2]);
    } else {
        M = 5;
        N = 5;
    }
    int i,j;
    int inc = 1;
    for(j = 0; j < M; j++) {
        for(i = 0; i < N; i++){
            printf("%d ", inc);
            inc++;
        }
        printf("\n");
    }


    //printf("M: %d, N: %d", M, N);
    while(M <= N)
    {
        return 0;
    }

    return 0;
}