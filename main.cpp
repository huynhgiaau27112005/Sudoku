#include <iostream>
#include "algo.h"
#include "fileworks.h"
#include "sudoku.h"

using namespace std;

int steps = 0;

int main()
{
	hideCursor();
	Sudoku sudoku = readFile("input3 - Copy.csv");
	printMatrix(sudoku);
	cout << endl << "Filled Cells: " << sudoku.countFilled << endl << endl;
	system("pause");
	bool isSolved = solve2(sudoku);
	if (isSolved == true)
	{
		system("cls");
		cout << "------------------------------------------------------------------------------------\n";
		cout << "Solved in " << steps << " steps!" << endl << endl;
		printMatrix(sudoku);
	}
	else cout << "Can not be solved." << endl;
	return 0;
}