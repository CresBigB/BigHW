#include "90-02-b1-magic_ball.h"

void fillEmptySlots(Matrix& matrix)
{
	if (matrix.x < 8) matrix.x = 8;
	if (matrix.x > 10) matrix.x = 10;
	if (matrix.y < 8) matrix.y = 8;
	if (matrix.y > 10) matrix.y = 10;
	srand(time(0));
	for (int i = 0; i < matrix.x; i++)
	{
		for (int j = 0; j < matrix.y + matrix.emptyColumn; j++)
			if (matrix.mat[i][j] == 0)
				matrix.mat[i][j] = rand() % 5 + 1;
	}
}

//void findElimination(const Matrix& matrix, Matrix tempMatrix)
//{
//	// 初始化tempMatrix.mat为0
//	tempMatrix.x = matrix.x;
//	tempMatrix.y = matrix.y;
//	for (int i = 0; i < tempMatrix.x; i++)
//		for (int j = 0; j < tempMatrix.y; j++)
//			tempMatrix.mat[i][j] = 0;
//
//	// 检查横向是否有超过3个连续相同的数字
//	for (int i = 0; i < 9; ++i)
//	{
//		for (int j = 0; j < 7; ++j)
//		{
//			if (matrix.mat[i][j] == matrix.mat[i][j + 1] && matrix.mat[i][j] == matrix.mat[i][j + 2])
//			{
//				tempMatrix.mat[i][j] = 1;
//				tempMatrix.mat[i][j + 1] = 1;
//				tempMatrix.mat[i][j + 2] = 1;
//			}
//		}
//	}
//
//	// 检查纵向是否有超过3个连续相同的数字
//	for (int j = 0; j < 9; ++j)
//	{
//		for (int i = 0; i < 7; ++i)
//		{
//			if (matrix.mat[i][j] == matrix.mat[i + 1][j] && matrix.mat[i][j] == matrix.mat[i + 2][j])
//			{
//				tempMatrix.mat[i][j] = 1;
//				tempMatrix.mat[i + 1][j] = 1;
//				tempMatrix.mat[i + 2][j] = 1;
//			}
//		}
//	}
//}

int DFS(int x, int y, int color, const Matrix& matrix, Matrix& tempMatrix)
{
	if (x >= matrix.x || y >= matrix.y || x < 0 || y < 0) return 0;
	if (tempMatrix.mat[x][y] || matrix.mat[x][y] != color || !matrix.mat[x][y]) return 0;

	int cnt = 0;
	constexpr int direction[4][2] = { {0,1},{1,0},{0,-1},{-1,0} };
	tempMatrix.mat[x][y] = 1;

	for (int i = 0; i < 4; ++i)
	{

		if (x + direction[i][0] >= 0 && x + direction[i][0] < matrix.x && y + direction[i][1] >= 0 && y + direction[i][1] < matrix.y)
			if (matrix.mat[x + direction[i][0]][y + direction[i][1]] == matrix.mat[x][y])
			{
				cnt += DFS(x + direction[i][0], y + direction[i][1], color, matrix, tempMatrix);
				cnt += tempMatrix.mat[x + direction[i][0]][y + direction[i][1]] = 1;
			}
	}
	tempMatrix.mat[x][y] = 0;
	return cnt;
}

int findElimination(int x, int y, const Matrix& matrix, Matrix& tempMatrix)
{
	int cnt = DFS(x, y, matrix.mat[x][y], matrix, tempMatrix);
	if (cnt < 1) return 0;
	tempMatrix.mat[x][y] = 1;
	return cnt;
}

int eliminateElements(Matrix& matrix, Matrix& tempMatrix)
{
	int cnt = 0;
	for (int i = 0; i < matrix.x; i++)
	{
		for (int j = 0; j < matrix.y + matrix.emptyColumn; j++)
			if (tempMatrix.mat[i][j] == 1)
			{
				++cnt;
				matrix.mat[i][j] = 0;
				tempMatrix.mat[i][j] = 0;
				tempMatrix.emptyColumn++;
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
			for (int j = 0; j < matrix.y + matrix.emptyColumn; j++)
				if (matrix.mat[i + 1][j] == 0)
					swap(matrix.mat[i][j], matrix.mat[i + 1][j]);
		}
	}
}

MouseAction getMouseAction()
{
	MouseAction mouse{};
	cct_read_keyboard_and_mouse(mouse.mouse_x, mouse.mouse_y, mouse.mouse_action, mouse.keyCode_1, mouse.keyCode_2);
	int x = mouse.mouse_x;
	mouse.mouse_x = (mouse.mouse_y + 1) / 2;
	mouse.mouse_y = (x + 2) / 4;
	return mouse;
}

int showMouseLocation(const Matrix& matrix, int x, int y)
{
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, matrix.x * 2 + 1);
	if (x > matrix.x || y > matrix.y + matrix.emptyColumn || x <= 0 || y <= 0)
		cout << "非法位置\t\t\n\t\t\t\t\n";
	else
	{
		cout << "鼠标当前行：" << x << "\t\n";
		cout << "鼠标当前列：" << y << "\t\n";
	}
	return 0;
}

void moveColumn(int idx, Matrix& matrix)
{
	if (idx <= 0 || idx > matrix.y) return;
	for (int i = 0; i < matrix.x; ++i)
		swap(matrix.mat[i][idx - 1], matrix.mat[i][idx]);
}

int getEmptyColumnIdx(const Matrix& matrix)
{
	for (int i = 0; i < matrix.y - 1; ++i)
	{
		int cnt = 0;
		for (int j = 0; j < matrix.x; ++j)
			if (matrix.mat[j][i]) break;
			else ++cnt;
		if (cnt == matrix.x) return i;
	}
	return -1;
}

void setEmpty(Matrix& matrix)
{
	for (int i = 0; i < matrix.x; ++i)
		for (int j = 0; j < matrix.y + matrix.emptyColumn; ++j)
			matrix.mat[i][j] = 0;
}

bool isGameOver(const Matrix& matrix)
{
	Matrix t{};
	t.x = matrix.x;
	t.y = matrix.y;
	for (int i = 0; i < matrix.x; ++i)
	{
		for (int j = 0; j < matrix.y + matrix.emptyColumn; ++j)
		{
			if (matrix.mat[i][j] != 0 &&
				DFS(i, j, matrix.mat[i][j], matrix, t) != 0)
				return false;
		}
	}
	return true;
}

int countItem(const Matrix& matrix)
{
	int cnt = 0;
	for (int i = 0; i < matrix.x; ++i)
		for (int j = 0; j < matrix.y; ++j)
			if (matrix.mat[i][j] != 0)
				++cnt;
	return cnt;
}

void keyHit(MouseAction& action)
{
	if (_kbhit())
	{
		int key = _getch();
		int x = action.mouse_x;
		int y = action.mouse_y;
		if (key == 72 || key == 80 || key == 75 || key == 77)
		{ // 方向键的前导字节
			switch (key)
			{
			case 72: // 上箭头
				break;
			case 80: // 下箭头
				y++;
				break;
			case 75: // 左箭头
				break;
			case 77: // 右箭头
				x++;
				break;
			}
			action.mouse_x = x;
			action.mouse_y = y;
		}
		if (key == 13)
		{
			action.mouse_action = MOUSE_LEFT_BUTTON_CLICK;
			cout << "H";
		}
	}
}
