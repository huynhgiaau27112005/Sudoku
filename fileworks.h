#pragma once
#include <iostream>
#include "sudoku.h"
using namespace std;

Sudoku readFile(const string& fileName);
void writeFile(const string& fileName, const Sudoku& sudoku);