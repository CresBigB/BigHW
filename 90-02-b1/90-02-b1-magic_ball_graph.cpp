#include <Windows.h>

#include "90-02-b1-magic_ball.h"
/*
	四种线型的框架，每种11个元素，每个元素均为一个中文字符（2字节）
	双线框架："╔", "╚", "╗", "╝", "═", "║", "╦", "╩", "╠", "╣", "╬"
	单线框架："┏", "┗", "┓", "┛", "━", "┃", "┳", "┻", "┣", "┫", "╋"
	横双竖单: "╒", "╘", "╕", "╛", "═", "│", "╤", "╧", "╞", "╡", "╪"
	横单竖双："╓", "╙", "╖", "╜", "─", "║", "╥", "╨", "╟", "╢", "╫"
*/

int getColor(int i)
{
	int color = COLOR_BLACK;
	switch (i)
	{
	case 1: color = COLOR_HYELLOW; break;
	case 2: color = COLOR_HBLUE; break;
	case 3: color = COLOR_HRED; break;
	case 4: color = COLOR_HGREEN; break;
	case 5: color = COLOR_HCYAN; break;
	}
	return color;
}

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
			{
				int color = 0;
				switch (matrix.mat[i - 1][j - 1])
				{
				case 1: color = COLOR_HYELLOW; break;
				case 2: color = COLOR_HBLUE; break;
				case 3: color = COLOR_HRED; break;
				case 4: color = COLOR_HGREEN; break;
				case 5: color = COLOR_HCYAN; break;
				}
				cct_showstr(j * 4 - 2, i * 2 - 1, "★", color, COLOR_BLACK);
			}
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
				cout << "━┛";
			else if (i == 0 && j == matrix.y + 1)
				cout << "━━━┓";
			else if (i == 0 && j < matrix.y)
				cout << "━━━━";
			else if (i == matrix.x + 1 && j != 0)
			{
				if (j == 1)
					cout << "━━";
				else
					cout << "━┻━━";
			}
			else if (j == 0 && i != 0 || j == matrix.y + 1 && i != 0)
			{
				cout << "┃";
				int x, y;
				cct_getxy(x, y);
				if (j == 0)
				{
					cct_showstr(x - 2, y + 1, "\n""┣━", COLOR_BLACK, COLOR_WHITE);
					for (int k = 0; k < matrix.y - 1; k++)
						cout << "━━╋━";
				}
				else if (i < matrix.x)
					cct_showstr(x - 2, y + 1, "━┫", COLOR_BLACK, COLOR_WHITE);
			}
			else if (i >= 1 && i <= matrix.x && j >= 1 && j <= matrix.y)
			{
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				int color = getColor(matrix.mat[i - 1][j - 1]);
				cct_showstr(j * 4 - 2, i * 2 - 1, "★", color, COLOR_BLACK);
				int x, y;
				cct_getxy(x, y);

				if (j < matrix.y)
					cct_showstr(x, y, "┃", COLOR_BLACK, COLOR_WHITE);

				if (i == 1 && (j > 0 && j < matrix.y))
					cct_showstr(x, y - 1, "┳", COLOR_BLACK, COLOR_WHITE);
			}
			//Sleep(20);
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
				cct_showstr((j + 1) * 4 - 2, (i + 1) * 2 - 1, "☆", getColor(matrix.mat[i][j]), COLOR_WHITE);
			else
				cct_showstr((j + 1) * 4 - 2, (i + 1) * 2 - 1, "★", getColor(matrix.mat[i][j]), COLOR_BLACK);

		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, max(matrix.x, matrix.y) * 2 + 1);
}

int useMouseSelectElement(Matrix& matrix, Matrix& tempMatrix, MouseAction& action)
{
	//auto act_1 = getMouseAction();
	//int y = act_1.mouse_y;
	//int x = act_1.mouse_x;
	//if (x <= matrix.x && y <= matrix.y && action.mouse_x <= matrix.x
	//	&& action.mouse_y <= matrix.y && (action.mouse_x != x || action.mouse_y != y))
	//{
	//	cct_showstr(y * 4 - 2, x * 2 - 1, "★", getColor(matrix.mat[x - 1][y - 1]), COLOR_WHITE);
	//	//cct_showstr(y * 4 - 2, x * 2 + 1, "★", getColor(matrix.mat[x - 1][y - 1]), COLOR_WHITE);
	//}
	//else if (x <= matrix.x && y <= matrix.y && action.mouse_x <= matrix.x && action.mouse_y <= matrix.y && matrix.mat[action.mouse_x][action.mouse_y])
	//	cct_showstr(action.mouse_y * 4 - 2, action.mouse_x * 2 - 1, "★",
	//		getColor(matrix.mat[action.mouse_x - 1][action.mouse_y - 1]), COLOR_WHITE);
	//tempMatrix.mat[x - 1][y - 1] = 1;
	//graph_setColorForElimination(matrix, tempMatrix);
	//auto act = getMouseAction();
	//if (act.mouse_x <= matrix.x && act.mouse_y <= matrix.y && matrix.mat[act.mouse_x - 1][act.mouse_y - 1])
	//	cct_showstr(act.mouse_y * 4 - 2, act.mouse_x * 2 - 1, "★",
	//		getColor(matrix.mat[act.mouse_x - 1][act.mouse_y - 1]), COLOR_WHITE);
	//
	//if (act.mouse_x != action.mouse_x || action.mouse_y != act.mouse_y && matrix.mat[act.mouse_x - 1][act.mouse_y - 1]
	//	&& (act.mouse_x <= matrix.x && act.mouse_y <= matrix.y && action.mouse_x <= matrix.x && action.mouse_y <= matrix.y))
	//	cct_showstr(action.mouse_y * 4 - 2, action.mouse_x * 2 - 1, "★",
	//		getColor(matrix.mat[action.mouse_x - 1][action.mouse_y - 1]), COLOR_BLACK);

	int cnt = 0;
	setEmpty(tempMatrix);
	static int i = 0;
	if (action.mouse_action == MOUSE_LEFT_BUTTON_CLICK)
	{
		cout << ++i;
		cnt = findElimination(action.mouse_x - 1, action.mouse_y - 1, matrix, tempMatrix);
		graph_setColorForElimination(matrix, tempMatrix);
	}

	showMouseLocation(matrix, 0, matrix.x * 2 + 1);
	return cnt;
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
						cct_showstr(j * 4 + 2, k * 2 + 1, "★", getColor(matrix.mat[k][j]), COLOR_BLACK);
						Sleep(20);
						cct_showstr(j * 4 + 2, i * 2 + 1, "★", getColor(matrix.mat[i][j]), COLOR_BLACK);
						break;
					}
				}
				Sleep(8);
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
				cct_showstr(j * 4 + 2, i * 2 + 1, "★", matrix.mat[i][j], COLOR_BLACK);
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
				cct_showstr(j * 4 + 2, i * 2 + 1, "◎", matrix.mat[i][j], COLOR_WHITE);
				++num;
			}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, max(matrix.x, matrix.y) * 2 + 1);
	return num;
}

int graph_eliminateElements(Matrix& matrix, Matrix& tempMatrix)
{
	int num = 0;
	for (int i = 0; i < matrix.x; i++)
	{
		for (int j = 0; j < matrix.y; j++)
		{
			if (tempMatrix.mat[i][j] == 1)
			{
				matrix.mat[i][j] = 0;
				tempMatrix.mat[i][j] = 0;
				tempMatrix.emptyColumn++;
				++num;
				cct_showstr(j * 4 + 2, i * 2 + 1, "★", getColor(matrix.mat[i][j]), COLOR_BLACK);
			}
			else
				cct_showstr(j * 4 + 2, i * 2 + 1, "★", getColor(matrix.mat[i][j]), COLOR_BLACK);
		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, max(matrix.x, matrix.y) * 2 + 1);
	return num;
}

void graph_moveColumn(int idx, Matrix& matrix)
{
	if (idx <= 0 || idx > matrix.y) return;
	for (int i = matrix.x - 1; i >= 0; --i)
	{
		cct_setcolor(COLOR_BLACK, COLOR_WHITE);
		cct_showstr(idx * 4 - 2, i * 2 + 1, "★", getColor(matrix.mat[i][idx]), COLOR_BLACK);
		Sleep(10);
		cct_showstr(idx * 4 + 2, i * 2 + 1, "  ", COLOR_BLACK, COLOR_BLACK);
		swap(matrix.mat[i][idx - 1], matrix.mat[i][idx]);
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
