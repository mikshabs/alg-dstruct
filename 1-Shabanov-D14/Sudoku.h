#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum {
	FALSE, TRUE
} bool_t;

typedef struct {
	int* field;
	int n;
	bool_t isSolved;
} sudoku_t;

bool_t checkCell(sudoku_t sudoku, int value, int cell);
void readField(sudoku_t* sudoku);
void writeField(sudoku_t sudoku);
void solveSudoku(sudoku_t* sudoku, int value, int cell);