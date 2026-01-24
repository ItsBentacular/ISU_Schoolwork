#include <stdio.h>

// an array that holds all possible movements that a knight can make (L shaped) in pairs of 2
int possible_moves[16] = { 1, 2, 1, -2, -1, 2, -1, -2, 2, 1, 2, -1, -2, 1, -2, -1};

// size of board (technically should work on any board size as long as these variables are changed.)
#define BOARD_ROWS 5
#define BOARD_COLUMNS 5

int line_count = 0; // test to check if lines equal a 5 by 5 board amount.

// array for the chess board
int board[BOARD_ROWS][BOARD_COLUMNS];

// array that holds the path the knight takes
int path[BOARD_ROWS * BOARD_COLUMNS];

//formula for row numbering: (row  * 5) + column + 1


// resets the board so it can be run through again
void board_reset() {
    for(int i = 0; i < BOARD_ROWS; i++)
    {
        for(int j = 0; j < BOARD_COLUMNS; j++)
        {
            board[i][j] = -1;
        }
    }
}


// function that recursively searches through the board for possible (valid) moves, stores the path tkaen in the path array, then prints the path.
void knights_path(int row, int column, int move_count) 
{
    int next_row;
    int next_column;
    path[move_count - 1] = (row * 5) + column + 1;

    if(move_count != BOARD_ROWS * BOARD_COLUMNS) {

        for(int i = 0; i < (sizeof(possible_moves) / sizeof(possible_moves[0])); i += 2) {

            next_row = row +  possible_moves[i];
            next_column = column + possible_moves[i + 1];
            if(next_row >= 0 && next_row < BOARD_ROWS && next_column >= 0 && next_column < BOARD_COLUMNS && board[next_row][next_column] == -1) {

                board[next_row][next_column] = move_count + 1;
                knights_path(next_row, next_column, move_count + 1);
                board[next_row][next_column] = -1;
            }

        }
    } 

    else if(move_count == BOARD_ROWS * BOARD_COLUMNS) {
        for(int k = 0; k < BOARD_ROWS * BOARD_COLUMNS; k++) {
            if(k < BOARD_ROWS * BOARD_COLUMNS - 1) {
                printf("%d, ", path[k]);
            } else {
                printf("%d ", path[k]);
                line_count++;
            }
        }
        printf("\n");
    }
}


int main(int argc, char *argv[]) {

    int i,j;

    for(i = 0; i < BOARD_ROWS; i++) {
        for(j = 0; j < BOARD_COLUMNS; j++) {
            board_reset();

            board[i][j] = 1; 
    
            knights_path(i, j, 1);
        }
    }
    printf("line count is: %d\n", line_count);
}