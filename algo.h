#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "fileworks.h"
using namespace std;

struct Cell
{
	int possible;
	int i;
	int j;
};

extern int steps;

bool setCheckFilled(Sudoku& sudoku);
bool solve1(Sudoku& sudoku);
bool solve2(Sudoku& sudoku);
void resetMatrix();
bool checkCellValid(Sudoku& sudoku, int i, int j, int number);
bool addNumberToMatrix(Sudoku& sudoku, int i, int j, int number, bool isInput);
void eraseCellValue(Sudoku& sudoku, int i, int j);