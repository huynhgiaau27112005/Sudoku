#include <iostream>
#include <windows.h>
#include <algorithm>
#include "algo.h"
#include "fileworks.h"

using namespace std;

bool setCheckFilled(Sudoku& sudoku) {
    vector<vector<int>> board = sudoku.Matrix;

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] != -1)
            {
                sudoku.markInput[i][j] = true;
                sudoku.countFilled++;
                //cout << i << " " << j << endl;
            }
        }
    }

    for (int i = 0; i < 9; i++)
    {
        vector<bool> checkRow (10, false);
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == -1) continue;
            else if (checkRow[board[i][j]] == true)
                return false;
            else
            {
                checkRow[board[i][j]] = true;
            }
        }
        sudoku.checkRow[i] = checkRow;
    }

    for (int j = 0; j < 9; j++)
    {
        vector<bool> checkCol(10, false);
        for (int i = 0; i < 9; i++)
        {
            if (board[i][j] == -1) continue;
            else if (checkCol[board[i][j]] == true)
                return false;
            else
            {
                checkCol[board[i][j]] = true;
            }
        }
        sudoku.checkCol[j] = checkCol;
    }

    int iCell[] = { 0, 0, 0, 3, 3, 3, 6, 6, 6 };
    int jCell[] = { 0, 3, 6, 0, 3, 6, 0, 3, 6 };
    for (int k = 0; k < 9; k++)
    {
        vector<bool> checkBlock(10, false);
        for (int i = iCell[k]; i <= iCell[k] + 2; i++)
        {
            for (int j = jCell[k]; j <= jCell[k] + 2; j++)
            {
                if (board[i][j] == -1) continue;
                else if (checkBlock[board[i][j]] == true)
                    return false;
                else
                {
                    checkBlock[board[i][j]] = true;
                }
            }
        }
        sudoku.checkBlock[k] = checkBlock;
    }
    return true;
}

int BlockID(int i, int j)
{
    if (i <= 2)
    {
        if (j <= 2) return 0;
        if (j <= 5) return 1;
        if (j <= 8) return 2;
    }
    if (i <= 5)
    {
        if (j <= 2) return 3;
        if (j <= 5) return 4;
        if (j <= 8) return 5;
    }
    if (i <= 8)
    {
        if (j <= 2) return 6;
        if (j <= 5) return 7;
        if (j <= 8) return 8;
    }
}

void backtrack1(Sudoku& sudoku, int i, int j, bool& isFinished)
{
    system("cls");
    cout << "------------------------------------------------------------------------------------\n";
    cout << i << " " << j << endl;
    printMatrix(sudoku);
    cout << endl;
    if (sudoku.countFilled == 81)
    {
        isFinished = true;
        return;
    }
    if (sudoku.Matrix[i][j] != -1)
    {
        int nextI = i, nextJ = j;
        if (nextJ == 8)
        {
            nextI++;
            nextJ = 0;
        }
        else
        {
            nextJ++;
        }
        return backtrack1(sudoku, nextI, nextJ, isFinished);
    }
    for (int number = 1; number <= 9; number++)
    {
        if (sudoku.checkRow[i][number] == false && sudoku.checkCol[j][number] == false && sudoku.checkBlock[BlockID(i, j)][number] == false)
        {
            sudoku.Matrix[i][j] = number;
            sudoku.checkRow[i][number] = true;
            sudoku.checkCol[j][number] = true;
            sudoku.checkBlock[BlockID(i, j)][number] = true;
            sudoku.countFilled++;

            int nextI = i, nextJ = j;
            if (nextJ == 8)
            {
                nextI++;
                nextJ = 0;
            }
            else
            {
                nextJ++;
            }
            backtrack1(sudoku, nextI, nextJ, isFinished);

            if (isFinished) return;

            sudoku.Matrix[i][j] = -1;
            sudoku.checkRow[i][number] = false;
            sudoku.checkCol[j][number] = false;
            sudoku.checkBlock[BlockID(i, j)][number] = false;
            sudoku.countFilled--;
        }
    }
}

bool solve1(Sudoku& sudoku)
{
    bool isFinished = false;
    backtrack1(sudoku, 0, 0, isFinished);
    return isFinished;
}

// SOLVE 2 ------------------------------------------------------------------------------------------------------------------------------

int countPossibleValues(const Sudoku& sudoku, int i, int j)
{
    int cnt = 0;
    for (int number = 1; number <= 9; number++)
    {
        if (sudoku.checkRow[i][number] == false && sudoku.checkCol[j][number] == false && sudoku.checkBlock[BlockID(i, j)][number] == false)
            cnt++;
    }
    return cnt;
}

bool comparator2(const Cell& a, const Cell& b)
{
    return a.possible < b.possible;
}

int steps = 0;
void backtrack2(Sudoku& sudoku, int index, const vector<Cell>& cellCollection, bool& isFinished)
{
    if (sudoku.countFilled == 81)
    {
        isFinished = true;
        return;
    }
    system("cls");
    cout << "------------------------------------------------------------------------------------\n";
    cout << "STEP: " << ++steps << endl;
    int i = cellCollection[index].i;
    int j = cellCollection[index].j;
    cout << "Fill (x,y): (" << i << "," << j << ")" << endl << endl;
    printMatrix(sudoku);
    cout << endl;
    for (int number = 1; number <= 9; number++)
    {
        if (sudoku.checkRow[i][number] == false && sudoku.checkCol[j][number] == false && sudoku.checkBlock[BlockID(i, j)][number] == false)
        {
            sudoku.Matrix[i][j] = number;
            sudoku.checkRow[i][number] = true;
            sudoku.checkCol[j][number] = true;
            sudoku.checkBlock[BlockID(i, j)][number] = true;
            sudoku.countFilled++;

            backtrack2(sudoku, index + 1, cellCollection, isFinished);

            if (isFinished) return;

            sudoku.Matrix[i][j] = -1;
            sudoku.checkRow[i][number] = false;
            sudoku.checkCol[j][number] = false;
            sudoku.checkBlock[BlockID(i, j)][number] = false;
            sudoku.countFilled--;
        }
    }
}

bool solve2(Sudoku& sudoku)
{
    vector<Cell> cellCollection;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (sudoku.Matrix[i][j] != -1)
                continue;
            Cell curCell;
            curCell.i = i;
            curCell.j = j;
            curCell.possible = countPossibleValues(sudoku, i, j);
            cellCollection.push_back(curCell);
        }
    }
    sort(cellCollection.begin(), cellCollection.end(), comparator2);
    /*for (Cell& c : cellCollection)
    {
        cout << c.i << " " << c.j << " : " << c.possible << endl;
    }
    system("pause");*/
    bool isFinished = false;
    backtrack2(sudoku, 0, cellCollection, isFinished);
    return isFinished;
}