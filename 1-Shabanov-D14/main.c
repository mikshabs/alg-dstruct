#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable: 4996)
#include "Sudoku.h"

int main(int argc, char* argv[]) {
	sudoku_t sudoku;
	readField(&sudoku);
	solveSudoku(&sudoku, 1, 0);
	if (sudoku.isSolved == TRUE) {
		writeField(sudoku);
	}
	else {
		FILE* file = fopen("output.txt", "w");
		fprintf(file, "0");
		fclose(file);
	}
	return 0;
}