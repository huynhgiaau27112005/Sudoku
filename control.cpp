#include "algo.h"
#include "fileworks.h"
#include "sudoku.h"
#include <windows.h>

using namespace std;

void hideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void initSudoku(Sudoku& sudoku)
{
	/*
	* Matrix: [0->8][0->8] stores the table's data, with -1 in empty cells.
	* checkCol, checkRow: index 0 -> 8 (true if the number is filled already)
	* checkBlock: index 0 -> 8 (blocks signed from left -> right, top -> bottom)
	* markInput: mark the position of input cells
	* countFilled: stores number of cells already filled.
	*/
	sudoku.countFilled = 0;
	for (int i = 0; i < 9; i++)
	{
		vector<int> tmp(9, 0);
		sudoku.Matrix.push_back(tmp);
		vector<bool> mark(9, 0);
		sudoku.markInput.push_back(mark);
		vector<bool> checkCell(10, false);
		sudoku.checkCol.push_back(checkCell);
		sudoku.checkRow.push_back(checkCell);
		sudoku.checkBlock.push_back(checkCell);
	}
}

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
void printMatrix(const Sudoku& sudoku)
{
	for (int i = 0; i < sudoku.Matrix.size(); i++)
	{
		for (int j = 0; j < sudoku.Matrix[i].size(); j++)
		{
			if (sudoku.Matrix[i][j] == -1)
				cout << ". ";
			else
			{
				if (sudoku.markInput[i][j] == true)
				{
					cout << sudoku.Matrix[i][j] << " ";
				}
				else cout << GREEN << sudoku.Matrix[i][j] << RESET << " ";
			}
		}
		cout << endl;
	}
}