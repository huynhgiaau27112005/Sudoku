#include <iostream>
#include "algo.h"
#include "fileworks.h"
#include "sudoku.h"
#include "raylib.h"
using namespace std;

int steps = 0;
vector<vector<Rectangle>> cellRectangle;
vector<Rectangle> numberRectangle;
vector<Rectangle> optionRectangle;
Rectangle solveButton{ 247.7, 600.1, 224.6, 68.7 };
Sudoku sudoku;
int numberStatus = 0;
Font font;

int main()
{
	/*hideCursor();
	Sudoku sudoku = readFile("input2.csv");
	printMatrix(sudoku);
	cout << endl << "Filled Cells: " << sudoku.countFilled << endl << endl;
	system("pause");
	bool isSolved = solve2(sudoku);
	if (isSolved == true)
	{
		cout << "Solved!" << endl;
		printMatrix(sudoku);
	}
	else cout << "Can not be solved." << endl;
	return 0;*/
	sudoku = readFile("empty.csv");
	setupVariables();
	mainBackground();
	return 0;
}