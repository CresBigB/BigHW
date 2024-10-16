#include "90-01-b2-magic_ball.h"

void fillEmptySlots(Matrix& matrix)
{
	if (matrix.x < 5) matrix.x = 5;
	if (matrix.x > 9) matrix.x = 9;
	if (matrix.y < 5) matrix.y = 5;
	if (matrix.y > 9) matrix.y = 9;
	srand(time(0));
	for (int i = 0; i < matrix.x; i++)
	{
		for (int j = 0; j < matrix.y; j++)
			if (matrix.mat[i][j] == 0)
				matrix.mat[i][j] = rand() % 9 + 1;
	}
}

void findElimination(const Matrix& matrix, Matrix& tempMatrix)
{
	// 初始化tempMatrix.mat为0
	tempMatrix.x = matrix.x;
	tempMatrix.y = matrix.y;
	for (int i = 0; i < tempMatrix.x; i++)
		for (int j = 0; j < tempMatrix.y; j++)
			tempMatrix.mat[i][j] = 0;

	// 检查横向是否有超过3个连续相同的数字
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			if (matrix.mat[i][j] == matrix.mat[i][j + 1] && matrix.mat[i][j] == matrix.mat[i][j + 2])
			{
				tempMatrix.mat[i][j] = 1;
				tempMatrix.mat[i][j + 1] = 1;
				tempMatrix.mat[i][j + 2] = 1;
			}
		}
	}

	// 检查纵向是否有超过3个连续相同的数字
	for (int j = 0; j < 9; ++j)
	{
		for (int i = 0; i < 7; ++i)
		{
			if (matrix.mat[i][j] == matrix.mat[i + 1][j] && matrix.mat[i][j] == matrix.mat[i + 2][j])
			{
				tempMatrix.mat[i][j] = 1;
				tempMatrix.mat[i + 1][j] = 1;
				tempMatrix.mat[i + 2][j] = 1;
			}
		}
	}
}

int eliminateElements(Matrix& matrix, const Matrix& tempMatrix)
{
	int cnt = 0;
	for (int i = 0; i < matrix.x; i++)
	{
		for (int j = 0; j < matrix.y; j++)
			if (tempMatrix.mat[i][j] == 1)
			{
				++cnt;
				matrix.mat[i][j] = 0;
			}
	}
	return cnt;
}

void fallTheBall(Matrix& matrix)
{
	for (int t = 0; t < matrix.x; ++t)
	{
		for (int i = 0; i < matrix.x - 1; i++)
		{
			for (int j = 0; j < matrix.y; j++)
				if (matrix.mat[i + 1][j] == 0)
					swap(matrix.mat[i][j], matrix.mat[i + 1][j]);
		}
	}
}

MouseAction getMouseAction()
{
	MouseAction mouse;
	cct_read_keyboard_and_mouse(mouse.mouse_x, mouse.mouse_y, mouse.mouse_action, mouse.keyCode_1, mouse.keyCode_2);
	return mouse;
}

int showMouseLocation(int x, int y)
{
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(x, y);
	auto action = getMouseAction();
	cout << "鼠标当前行：" << (action.mouse_y + 1) / 2 << "\t\n";
	cout << "鼠标当前列：" << (action.mouse_x + 2) / 4 << "\t\n";
	return action.mouse_action;
}