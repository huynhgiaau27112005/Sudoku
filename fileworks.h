#pragma once
#include <iostream>
#include "sudoku.h"
using namespace std;

#define TEXTURE_LINK "Textures\\"

Sudoku readFile(const string& fileName);
void writeFile(const string& fileName, const Sudoku& sudoku);