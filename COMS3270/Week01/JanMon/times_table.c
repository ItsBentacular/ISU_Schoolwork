#include <stdio.h>

#define SIZE 30

int table[SIZE][SIZE];

int populate_table(int table[SIZE][SIZE]){
	
// the local table shadows the global table
int i,j;

	for(j = 0; j< SIZE; j++) {
		for(i = 0; i < SIZE; i++) {
			table[j][i] = (i + 1) * (j + 1);
		}
	}
	
	return 0;
}

int print_table(int table[SIZE][SIZE]){
	int i,j;
	for (j = 0; j < SIZE; j ++) {
		for(i = 0; i < SIZE; i++) {
			printf("%d   ", table[j][i]);
		}
		printf("\n");
	}
	return 0;
}

int main(int argc, char *argv[]) {
	populate_table(table);
	print_table(table);
	return 0;
}