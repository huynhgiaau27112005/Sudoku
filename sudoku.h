#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

struct Sudoku
{
	vector<vector<int>> Matrix;
	vector<vector<bool>> checkRow;
	vector<vector<bool>> checkCol;
	vector<vector<bool>> checkBlock;
	vector<vector<bool>> markInput;
	int countFilled = 0;
};

void initSudoku(Sudoku& sudoku);
void printMatrix(const Sudoku& sudoku);
void hideCursor();