#include <stdio.h>

typedef enum op {
    read_text,
    read_binary,
    write_text,
    write_binary,
    bad_switch
} operation;
/* Program will take one of four switches, -rt, -rb, -wt, -wb. these specify that
 * the program should read text, read binary, etc..
*/

typedef struct {
    int i;
    int j;
} data;


int main(int argc, char *argv[]) {
    operation op;
    data d;
    FILE *f;

    if(argc != 2) {
        //print usage message
        printf("error, need args");
        return -1;
    }

    if(argv[1][0] == '-') {
        if(argv[1][1] == 'r') {
            if(argv[1][2] == 't') {
                op = read_text;
            } else if(argv[1][2] == 'b') {
                op = read_binary;
            }
        }
        else if(argv[1][1] == 'w') {
            if(argv[1][2] == 't') {
                op = write_text;
            } else if(argv[1][2] == 'b') {
                op = write_binary;
            }
        }
    }
    d.i = 1;
    d.j = 290222;
    if(op == bad_switch) {
        printf("badswitch");
        return -1;
    }

    switch(op) {
        case read_text:
        if(!(f = fopen("textfile", "r"))) {
            fprintf(stderr, "failed to open file for reading\n");
            return -1;
        }

        if(fscanf(f, "%d %d\n", &d.i, &d.j) != 2) {
            fprintf(stderr,"error");
        } else {
            printf("%d %d\n", d.i, d.j);
        }
        break;
        case write_text:
        if(!(f = fopen("textfile", "w" ))) {
            fprintf(stderr, "Failed to open file for writing.\n");
            return -1;
        }

        fprintf(f, "%d %d\n", d.i, d.j);
        break;
        case read_binary:
        if(!(f = fopen("binaryfile", "r"))) {
            fprintf(stderr, "Failed to open file for reading.\n");
            return -1;
        }

       if(fread(&d, sizeof(d), 1, f) != 1) {
        fprintf(stderr, "read error\n");
       }

       printf("%d %d\n", d.i, d.j);
        break;
        case write_binary:
        if(!(f = fopen("binaryfile", "w"))) {
            fprintf(stderr, "Failed to open file for writing.\n");
            return -1;
        }

        (fwrite(&d, sizeof(d), 1, f) != 1);
        break;
        case bad_switch:
        printf("errorbadswitch");
        return -1;
        break;
    }

    fclose(f);

    return 0;
}