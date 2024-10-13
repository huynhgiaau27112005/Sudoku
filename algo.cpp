#include <iostream>
#include <algorithm>
#include "algo.h"
#include "fileworks.h"
#include "sudoku.h"

using namespace std;

bool setCheckFilled(Sudoku& sudoku) {
    vector<vector<int>> board = sudoku.Matrix;
    sudoku.countFilled = 0;

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
    /*system("cls");
    cout << "------------------------------------------------------------------------------------\n";
    cout << i << " " << j << endl;
    printMatrix(sudoku);
    cout << endl;*/
    ++steps;
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
vector<vector<int>> cellOptions(9, vector<int>(9, 0)); // store the number of possible values of each cell
vector<vector<vector<bool>>> cellPossibleValues(9, vector<vector<bool>>(9, vector<bool>(10, false)));
int countPossibleValues(const Sudoku& sudoku, int i, int j)
{
    int cnt = 0;
    for (int number = 1; number <= 9; number++)
    {
        if (sudoku.checkRow[i][number] == false && sudoku.checkCol[j][number] == false && sudoku.checkBlock[BlockID(i, j)][number] == false)
        {
            cellPossibleValues[i][j][number] = true;
            cnt++;
        }
    }
    return cnt;
}

vector<Cell> updateCellsPossibleValue(Sudoku& sudoku, int i, int j, int cellValue)
{
    vector<Cell> updatedCells;
    /*bool valueisUsed;
    if (amount == -1) valueisUsed = true;
    else valueisUsed = false;*/

    int blockI[] = { 0, 0, 0, 3, 3, 3, 6, 6, 6 };
    int blockJ[] = { 0, 3, 6, 0, 3, 6, 0, 3, 6 };
    int blockID = BlockID(i, j);
    int startI = blockI[blockID], startJ = blockJ[blockID];
    for (int di = startI; di <= startI + 2; di++)
    {
        for (int dj = startJ; dj <= startJ + 2; dj++)
        {
            if (cellPossibleValues[di][dj][cellValue] == true)
            {
                updatedCells.push_back({ 0, di, dj });
                cellPossibleValues[di][dj][cellValue] = false;
                cellOptions[di][dj]--;
            }
        }
    }
    for (int k = 0; k < 9; k++)
    {
        if (k < startJ || k > startJ + 2)
        {
            if (cellPossibleValues[i][k][cellValue] == true)
            {
                updatedCells.push_back({ 0, i, k });
                cellPossibleValues[i][k][cellValue] = false;
                cellOptions[i][k]--;
            }
        }

        if (k < startI || k > startI + 2)
        {
            if (cellPossibleValues[k][j][cellValue] == true)
            {
                updatedCells.push_back({ 0, k, j });
                cellPossibleValues[k][j][cellValue] = false;
                cellOptions[k][j]--;
            }
        }
    }
    return updatedCells;
}

bool comparator2(const Cell& a, const Cell& b)
{
    return cellOptions[a.i][a.j] < cellOptions[b.i][b.j];
}

int getUniqueValueOfCell(Sudoku& sudoku, int i, int j)
{
    for (int number = 1; number <= 9; number++)
    {
        if (cellPossibleValues[i][j][number] == false) continue;
        bool isUnique = true;

        // Check Block
        int blockI[] = { 0, 0, 0, 3, 3, 3, 6, 6, 6 };
        int blockJ[] = { 0, 3, 6, 0, 3, 6, 0, 3, 6 };
        int blockID = BlockID(i, j);
        int startI = blockI[blockID], startJ = blockJ[blockID];
        for (int di = startI; di <= startI + 2; di++)
        {
            for (int dj = startJ; dj <= startJ + 2; dj++)
            {
                if (di == i && dj == j) continue;
                if (cellPossibleValues[i][j][number] == true)
                {
                    isUnique = false;
                    break;
                }
            }
            if (!isUnique) break;
        }
        if (!isUnique) continue;

        // Check Row
        for (int k = 0; k < 9; k++)
        {
            if (k != j)
            {
                if (cellPossibleValues[i][k][number] == true)
                {
                    isUnique = false;
                    break;
                }
            }
            if (k != i)
            {
                if (cellPossibleValues[k][j][number] == true)
                {
                    isUnique = false;
                    break;
                }
            }
            if (!isUnique) break;
        }
        if (!isUnique) continue;
        else return number;
    }
    return -1;
}

void backtrack2(Sudoku& sudoku, int index, vector<Cell>& cellCollection, bool& isFinished)
{
    if (sudoku.countFilled == 81)
    {
        isFinished = true;
        return;
    }

    int i = cellCollection[index].i;
    int j = cellCollection[index].j;

    ++steps;
    /*system("cls");
    //cout << "------------------------------------------------------------------------------------\n";
    //cout << "STEP: " << ++steps << endl;
    //cout << "Fill (x,y): (" << i << "," << j << ")" << endl << endl;
    //printMatrix(sudoku);

    for (int k = index; k < cellCollection.size(); k++)
    {
        cout << cellCollection[k].i << " " << cellCollection[k].j << " : " << cellOptions[cellCollection[k].i][cellCollection[k].j] << endl;
    }
    //int cellsLeft = max(0, cellCollection.size() - 1 - index - 10);
    //if (cellsLeft > 0) cout << cellsLeft << " more..." << endl;

    //cout << endl;*/
    for (int number = 1; number <= 9; number++)
    {
        if (cellPossibleValues[i][j][number] == true)
        {
            sudoku.Matrix[i][j] = number;
            sudoku.checkRow[i][number] = true;
            sudoku.checkCol[j][number] = true;
            sudoku.checkBlock[BlockID(i, j)][number] = true;
            sudoku.countFilled++;
            vector<Cell> updatedCells = updateCellsPossibleValue(sudoku, i, j, number);

            sort(cellCollection.begin() + index + 1, cellCollection.end(), comparator2);
            backtrack2(sudoku, index + 1, cellCollection, isFinished);

            if (isFinished) return;

            //updateCellsPossibleValue(sudoku, i, j, number, 1);
            for (const Cell& cell : updatedCells)
            {
                cellPossibleValues[cell.i][cell.j][number] = true;
                cellOptions[cell.i][cell.j]++;
            }
            //---------------------------------------------------------

            sudoku.Matrix[i][j] = -1;
            sudoku.checkRow[i][number] = false;
            sudoku.checkCol[j][number] = false;
            sudoku.checkBlock[BlockID(i, j)][number] = false;
            sudoku.countFilled--;

            /*sudoku.Matrix[i][j] = number;
            sudoku.checkRow[i][number] = true;
            sudoku.checkCol[j][number] = true;
            sudoku.checkBlock[BlockID(i, j)][number] = true;
            sudoku.countFilled++;

            vector<vector<vector<bool>>> firstCellPossibleValues = cellPossibleValues;
            vector<vector<int>> firstCellOptions = cellOptions;
            updateCellsPossibleValue(sudoku, i, j, number);
            for (int di = 0; di < 9; i++)
            {
                for (int dj = 0; dj < 9; dj++)
                {
                    int uniqueNumber = getUniqueValueOfCell(sudoku, di, dj);
                    if (uniqueNumber != -1)
                    {
                        for (int k = 1; k <= 9; k++)
                            cellPossibleValues[di][dj][k] = false;
                        cellPossibleValues[di][dj][uniqueNumber] = true;
                        cellOptions[di][dj] = 1;
                    }
                }
            }

            sort(cellCollection.begin() + index + 1, cellCollection.end(), comparator2);
            backtrack2(sudoku, index + 1, cellCollection, isFinished);

            if (isFinished) return;

            cellPossibleValues = firstCellPossibleValues;
            cellOptions = firstCellOptions;

            sudoku.Matrix[i][j] = -1;
            sudoku.checkRow[i][number] = false;
            sudoku.checkCol[j][number] = false;
            sudoku.checkBlock[BlockID(i, j)][number] = false;
            sudoku.countFilled--;*/
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
            //curCell.possible = countPossibleValues(sudoku, i, j);
            cellOptions[i][j] = countPossibleValues(sudoku, i, j);
            cellCollection.push_back(curCell);
        }
    }
    sort(cellCollection.begin(), cellCollection.end(), comparator2);
    /*for (Cell& c : cellCollection)
    {
        cout << c.i << " " << c.j << " : " << cellOptions[c.i][c.j] << endl;
    }
    system("pause");*/
    bool isFinished = false;
    backtrack2(sudoku, 0, cellCollection, isFinished);
    if (!isFinished) cout << "can not solve" << endl;
    return isFinished;
}

bool checkCellValid(Sudoku& sudoku, int i, int j, int number)
{
    if (sudoku.checkRow[i][number] == true) return false;
    if (sudoku.checkCol[j][number] == true) return false;
    if (sudoku.checkBlock[BlockID(i, j)][number] == true) return false;
    return true;
}

bool addNumberToMatrix(Sudoku& sudoku, int i, int j, int number, bool isInput)
{
    if (!checkCellValid(sudoku, i, j, number)) return false;

    sudoku.Matrix[i][j] = number;
    sudoku.checkRow[i][number] = true;
    sudoku.checkCol[j][number] = true;
    sudoku.checkBlock[BlockID(i, j)][number] = true;
    sudoku.countFilled++;
    sudoku.markInput[i][j] = isInput;
}

void eraseCellValue(Sudoku& sudoku, int i, int j)
{
    int number = sudoku.Matrix[i][j];
    if (number == -1) return;

    sudoku.Matrix[i][j] = -1;
    sudoku.checkRow[i][number] = false;
    sudoku.checkCol[j][number] = false;
    sudoku.checkBlock[BlockID(i, j)][number] = false;
    sudoku.countFilled--;
    sudoku.markInput[i][j] = false;
    // set all non-empty cells into 'input cells'
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (sudoku.Matrix[i][j] != -1)
            {
                sudoku.markInput[i][j] = true;
            }
        }
    }
}


void resetMatrix()
{
    sudoku.countFilled = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            sudoku.Matrix[i][j] = -1;
            sudoku.markInput[i][j] = false;
        }
    }
    for (int i = 0; i < 9; i++)
    {
        for (int number = 1; number <= 9; number++)
        {
            sudoku.checkCol[i][number] = false;
            sudoku.checkRow[i][number] = false;
            sudoku.checkBlock[i][number] = false;
        }
    }
    steps = 0;
}