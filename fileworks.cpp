#include <iostream>
#include <fstream>
#include <string>
#include "fileworks.h"
#include "algo.h"
#include "sudoku.h"

using namespace std;

Sudoku readFile(const string& fileName)
{
	Sudoku sudoku;
	initSudoku(sudoku);
	fstream f(fileName.c_str(), ios::in);
	if (!f.is_open())
	{
		cout << "Can not open file \'" << fileName << "\'." << endl;
		Sudoku empty;
		return empty;
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			char c;
			f >> c;
			if ('1' <= c && c <= '9')
			{
				sudoku.Matrix[i][j] = c - '0';
				f.ignore();
			}
			else if (c == ',')
			{
				sudoku.Matrix[i][j] = -1;
			}
		}
		f.ignore();
	}
	f.close();
	if (setCheckFilled(sudoku) == false)
	{
		cout << "Invalid Sudoku.";
		exit(0);
	}
	return sudoku;
}