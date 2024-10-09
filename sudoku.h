#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "raylib.h"

using namespace std;

#define NUMBER_STATUS_NONE 0
#define NUMBER_STATUS_ERASE 10
#define FONT_NUMBER_SIZE 45

struct Sudoku
{
	vector<vector<int>> Matrix;
	vector<vector<bool>> checkRow;
	vector<vector<bool>> checkCol;
	vector<vector<bool>> checkBlock;
	vector<vector<bool>> markInput;
	int countFilled = 0;
};

extern vector<vector<Rectangle>> cellRectangle;
extern vector<Rectangle> numberRectangle;
extern vector<Rectangle> optionRectangle;
extern Rectangle solveButton;
extern int numberStatus;
extern Sudoku sudoku;
extern Font font;

void initSudoku(Sudoku& sudoku);
void printMatrix(const Sudoku& sudoku);
//void hideCursor();
void mainBackground();
void setupVariables();
void displayMatrix();