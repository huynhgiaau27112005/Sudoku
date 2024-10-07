#include <iostream>
#include "algo.h"
#include "fileworks.h"
#include "sudoku.h"

using namespace std;

int main()
{
	hideCursor();
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
	return 0;
}