#include <Windows.h>

#include "90-01-b2-magic_ball.h"
/*
	四种线型的框架，每种11个元素，每个元素均为一个中文字符（2字节）
	双线框架："╔", "╚", "╗", "╝", "═", "║", "╦", "╩", "╠", "╣", "╬"
	单线框架："┏", "┗", "┓", "┛", "━", "┃", "┳", "┻", "┣", "┫", "╋"
	横双竖单: "╒", "╘", "╕", "╛", "═", "│", "╤", "╧", "╞", "╡", "╪"
	横单竖双："╓", "╙", "╖", "╜", "─", "║", "╥", "╨", "╟", "╢", "╫"
*/


void drawGraph_No_Border(const Matrix& matrix)
{
	cct_gotoxy(0, 0);
	int size = max(matrix.x, matrix.y);
	cct_setconsoleborder(size + 2 * 19, size + 2 * 8, size + 2 * 19, size + 2 * 8);
	cct_setfontsize("新宋体", 40, 20);
	for (int i = 0; i < matrix.x + 2; i++)
	{
		for (int j = 0; j < matrix.y + 2; j++)
		{
			cct_setcolor(COLOR_BLACK, COLOR_WHITE);
			if (i == 0 && j == 0)
				cout << "┏";
			else if (i == matrix.x + 1 && j == 0)
				cout << "┗";
			else if (i == matrix.x + 1 && j == matrix.y + 1)
				cout << "━━━┛";
			else if (i == 0 && j == matrix.y + 1)
				cout << "━━━┓";
			else if (i == 0 && j < matrix.y || i == matrix.x + 1 && j < matrix.y)
				cout << "━━━━";
			else if (j == 0 && i != 0 || j == matrix.y + 1 && i != 0)
			{
				cout << "┃";
				int x, y;
				cct_getxy(x, y);
				if (j == 0)
				{
					cct_showstr(x - 2, y + 1, "\n""┃", COLOR_BLACK, COLOR_WHITE);

				}
				else if (i < matrix.x)
					cct_showstr(x - 2, y + 1, " ┃", COLOR_BLACK, COLOR_WHITE);
			}
			else if (i >= 1 && i <= matrix.x && j >= 1 && j <= matrix.y)
				cct_showstr(j * 4 - 2, i * 2 - 1, "○",
					matrix.mat[i - 1][j - 1] == COLOR_BLUE ? COLOR_HGREEN : matrix.mat[i - 1][j - 1], COLOR_BLACK);
			Sleep(20);
		}
		cout << "\n";
	}
}

void drawGraph_With_Border(const Matrix& matrix)
{
	cct_gotoxy(0, 0);
	int size = max(matrix.x, matrix.y);
	cct_setconsoleborder(size + 2 * 19, size + 2 * 8, size + 2 * 19, size + 2 * 8);
	cct_setfontsize("新宋体", 40, 20);
	for (int i = 0; i < matrix.x + 2; i++)
	{
		for (int j = 0; j < matrix.y + 2; j++)
		{
			cct_setcolor(COLOR_BLACK, COLOR_WHITE);
			if (i == 0 && j == 0)
				cout << "┏";
			else if (i == matrix.x + 1 && j == 0)
				cout << "┗";
			else if (i == matrix.x + 1 && j == matrix.y + 1)
				cout << "┛";
			else if (i == 0 && j == matrix.y + 1)
				cout << "━━━┓";
			else if (i == 0 && j < matrix.y)
				cout << "━━━━";
			else if (i == matrix.x + 1 && j != 0)
			{
				if (j == 1)
					cout << "━━━";
				else
					cout << "┻━━━";
			}
			else if (j == 0 && i != 0 || j == matrix.y + 1 && i != 0)
			{
				cout << "┃";
				int x, y;
				cct_getxy(x, y);
				if (j == 0)
				{
					cct_showstr(x - 2, y + 1, "\n""┣━━", COLOR_BLACK, COLOR_WHITE);
					for (int k = 0; k < matrix.y - 1; k++)
						cout << "━╋━━";
				}
				else if (i < matrix.x)
					cct_showstr(x - 2, y + 1, "━┫", COLOR_BLACK, COLOR_WHITE);
			}
			else if (i >= 1 && i <= matrix.x && j >= 1 && j <= matrix.y)
			{
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				cct_showstr(j * 4 - 2, i * 2 - 1, "○", matrix.mat[i - 1][j - 1], COLOR_BLACK);
				int x, y;
				cct_getxy(x, y);

				if (j < matrix.y)
					cct_showstr(x, y, "┃", COLOR_BLACK, COLOR_WHITE);

				if (i == 1 && (j > 0 && j < matrix.y))
					cct_showstr(x, y - 1, "┳━", COLOR_BLACK, COLOR_WHITE);
			}
			Sleep(20);
		}
		cout << "\n";
	}
}

void graph_setColorForElimination(const Matrix& matrix, const Matrix& tempMatrix)
{
	for (int i = 0; i < matrix.x; ++i)
	{
		for (int j = 0; j < matrix.y; ++j)
		{
			if (tempMatrix.mat[i][j])
				cct_showstr((j + 1) * 4 - 2, (i + 1) * 2 - 1, "●", COLOR_HPINK, COLOR_BLACK);
		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, max(matrix.x, matrix.y) * 2 + 1);
}

int useMouseSwapElement(Matrix& matrix)
{
	auto act_1 = getMouseAction();
	int y1 = (act_1.mouse_x + 2) / 4;
	int x1 = (act_1.mouse_y + 1) / 2;
	if (act_1.mouse_action == MOUSE_LEFT_BUTTON_CLICK)
	{
		int x2;
		int y2;
		while (true)
		{
			auto act_2 = getMouseAction();
			if (act_2.mouse_action == MOUSE_LEFT_BUTTON_CLICK)
			{
				y2 = (act_2.mouse_x + 2) / 4;
				x2 = (act_2.mouse_y + 1) / 2;
				break;
			}
		}
		if (x2 == x1 && y2 == y1 + 1 || x2 == x1 && y2 == y1 - 1 || x2 == x1 + 1 && y2 == y1 || x2 == x1 - 1 && y2 == y1)
		{
			swap(matrix.mat[x1 - 1][y1 - 1], matrix.mat[x2 - 1][y2 - 1]);
			cct_showstr(y1 * 4 - 2, x1 * 2 - 1, "○", matrix.mat[x1 - 1][y1 - 1], COLOR_BLACK);
			cct_showstr(y2 * 4 - 2, x2 * 2 - 1, "○", matrix.mat[x2 - 1][y2 - 1], COLOR_BLACK);
			return MOUSE_LEFT_BUTTON_CLICK;
		}
	}
	showMouseLocation(0, matrix.x * 2 + 1);
	if (act_1.mouse_action == MOUSE_RIGHT_BUTTON_CLICK)
		return MOUSE_RIGHT_BUTTON_CLICK;
	return MOUSE_ONLY_MOVED;
}

void graph_fallTheBall(Matrix& matrix)
{
	for (int j = 0; j < matrix.y; ++j)
	{
		for (int i = matrix.x - 1; i > 0; --i)
		{
			if (matrix.mat[i][j] == 0)
			{
				for (int k = i - 1; k >= 0; --k)
				{
					if (matrix.mat[k][j] != 0)
					{
						cct_setcolor(COLOR_BLACK, COLOR_WHITE);
						swap(matrix.mat[i][j], matrix.mat[k][j]);
						Sleep(20);
						cct_showstr(j * 4 + 2, k * 2 + 1, "○", matrix.mat[k][j], COLOR_BLACK);
						Sleep(20);
						cct_showstr(j * 4 + 2, i * 2 + 1, "○", matrix.mat[i][j], COLOR_BLACK);
						break;
					}
				}
				Sleep(20);
			}
		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}

void graph_fillEmptySlots(Matrix& matrix)
{
	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < matrix.x; i++)
	{
		for (int j = 0; j < matrix.y; j++)
			if (matrix.mat[i][j] == 0)
			{
				matrix.mat[i][j] = rand() % 9 + 1;
				cct_showstr(j * 4 + 2, i * 2 + 1, "○", matrix.mat[i][j], COLOR_BLACK);
			}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}

int graph_markPotentialEliminations(const Matrix& matrix, Matrix& tempMatrix)
{
	markPotentialEliminations(matrix, tempMatrix);
	int num = 0;
	for (int i = 0; i < tempMatrix.x; i++)
	{
		for (int j = 0; j < tempMatrix.y; j++)
			if (tempMatrix.mat[i][j] == 1)
			{
				cct_showstr(j * 4 + 2, i * 2 + 1, "◎", matrix.mat[i][j], COLOR_BLACK);
				++num;
			}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, max(matrix.x, matrix.y) * 2 + 1);
	return num;
}

int graph_eliminateElements(Matrix& matrix, const Matrix& tempMatrix)
{
	int num = 0;
	for (int i = 0; i < matrix.x; i++)
	{
		for (int j = 0; j < matrix.y; j++)
		{
			if (tempMatrix.mat[i][j] == 1)
			{
				matrix.mat[i][j] = 0;
				++num;
				cct_showstr(j * 4 + 2, i * 2 + 1, "◎", matrix.mat[i][j], COLOR_BLACK);
			}
			else
				cct_showstr(j * 4 + 2, i * 2 + 1, "○", matrix.mat[i][j], COLOR_BLACK);
		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, max(matrix.x, matrix.y) * 2 + 1);
	return num;
}