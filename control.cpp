#include "algo.h"
#include "fileworks.h"
#include "sudoku.h"
#include <string>
#include "raylib.h"
#include <thread>

using namespace std;

/*void hideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}*/

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

void setupVariables()
{
	// Set up cellRectangle (store cell positions in Matrix)
	const float cellWidth = 47;
	const float cellHeight = 45;
	const float gapWidth = 48.2;
	const float gapHeight = 46.2;
	const float gapBlockWidth = 51.3;
	const float gapBlockHeight = 49.3;
	float curY = 94.3;
	vector<vector<Rectangle>> tmp(9, vector<Rectangle>(9));
	cellRectangle = tmp;
	for (int y = 0; y < 9; y++)
	{
		if (y != 0)
		{
			if (y == 3 || y == 6)
				curY += gapBlockHeight;
			else
				curY += gapHeight;
		}
		float curX = 140.7;
		for (int x = 0; x < 9; x++)
		{
			if (x != 0)
			{
				if (x == 3 || x == 6)
					curX += gapBlockWidth;
				else
					curX += gapWidth;
			}
			Rectangle curRec;
			curRec.width = cellWidth;
			curRec.height = cellHeight;
			curRec.x = curX;
			curRec.y = curY;
			cout << curRec.x << " " << curRec.y << endl;
			cellRectangle[y][x] = curRec;
		}
	}

	// Set up numberRectangle (store number button (0 -> 9) positions)
	const float buttonWidth = 42;
	const float buttonHeight = 42;
	const float gapX = 48;
	const float gapXBig = 52;
	curY = 539.1;
	float curX = 143.2;
	vector<Rectangle> tmp2(10);
	numberRectangle = tmp2;
	for (int i = 0; i < 9; i++)
	{
		if (i != 0 && i == 3 || i == 6)
			curX += gapXBig;
		else if (i != 0)
			curX += gapX;
		Rectangle curRec;
		curRec.x = curX;
		curRec.y = curY;
		curRec.width = buttonWidth;
		curRec.height = buttonHeight;
		numberRectangle[i] = curRec;
	}
	numberRectangle[9] = { 591.6, 539.1, buttonWidth, buttonHeight }; // erase button

	// Set up optionRectangle (store option buttons (reset matrix, upload matrix, download matrix) positions)
	optionRectangle.push_back({ 479.9, 46.1, 31.9, 31.9 });
	optionRectangle.push_back({ 515.8, 46.1, 31.9, 31.9 });
	optionRectangle.push_back({ 551.7, 46.1, 31.9, 31.9 });
}

void displayMatrix()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			string number = to_string(sudoku.Matrix[i][j]);
			if (number != "-1")
			{
				Vector2 textSize = MeasureTextEx(font, number.c_str(), FONT_NUMBER_SIZE, 2);
				Rectangle cellRec = cellRectangle[i][j];
				float textPosX = cellRec.x + (cellRec.width - textSize.x) / 2 - 1;
				float textPosY = cellRec.y + (cellRec.height - textSize.y) / 2;

				if (sudoku.markInput[i][j] == true)
				{
					DrawTextEx(font, number.c_str(), { textPosX, textPosY }, FONT_NUMBER_SIZE, 2, BLACK);
				}
				else
				{
					DrawTextEx(font, number.c_str(), { textPosX, textPosY }, FONT_NUMBER_SIZE, 2, DARKBLUE);
				}
			}
		}
	}
}

void mainBackground()
{
	const int screenWidth = 720;
	const int screenHeight = 720;
	SetTargetFPS(60);
	InitWindow(screenWidth, screenHeight, "Sudoku Solver");
	Texture2D background = LoadTexture("Textures\\main_background.png"); // Store image file to Texture2D datatype

	bool needExit = false;

	font = LoadFontEx("Font\\FrancoisOne-Regular.ttf", FONT_NUMBER_SIZE, NULL, 0);
	SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(SKYBLUE);
		DrawTexture(background, 0, 0, WHITE); // Draw Texture to position (0, 0) in the screen, "WHITE" is always used

		DrawTextEx(font, to_string(steps).c_str(), { 214.3, 39.5 }, 42, 2, { 0, 49, 176, 255 });

		displayMatrix();
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				Rectangle curRec = cellRectangle[i][j];
				if (CheckCollisionPointRec(GetMousePosition(), curRec))
				{
					DrawRectangleRec(curRec, Fade(DARKGREEN, 0.2f));
				}
			}
		}

		for (int i = 0; i < 10; i++)
		{
			if (CheckCollisionPointRec(GetMousePosition(), numberRectangle[i]))
			{
				DrawRectangleRec(numberRectangle[i], Fade(DARKGREEN, 0.2f));
			}
		}

		for (int i = 0; i < 3; i++)
		{
			if (CheckCollisionPointRec(GetMousePosition(), optionRectangle[i]))
			{
				DrawRectangleRec(optionRectangle[i], Fade(DARKGREEN, 0.2f));
			}
		}

		if (CheckCollisionPointRec(GetMousePosition(), solveButton))
		{
			if (CheckCollisionPointRec(GetMousePosition(), solveButton))
			{
				DrawRectangleRec(solveButton, Fade(DARKGREEN, 0.2f));
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
					thread t_solve(solve2, ref(sudoku));
					//solve2(sudoku);
					t_solve.detach();
				}
			}
		}

		EndDrawing();

		if (needExit) break;

	}
	UnloadFont(font);
	UnloadTexture(background); // Deallocate texture -> free the memory after finish the program.
	CloseWindow();
}