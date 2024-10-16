#include "90-01-b2-magic_ball.h"

void showMatrix(const Matrix& matrix)
{
	cct_gotoxy(0, 0);
	system("cls");
	for (int i = 0; i < matrix.x; i++)
	{
		for (int j = 0; j < matrix.y; j++)
			cout << matrix.mat[i][j] << " ";
		cout << "\n";
	}
	cct_gotoxy(0, max(matrix.x, matrix.y) + 1);
}

void setColorForElimination(const Matrix& matrix, const Matrix& tempMatrix)
{
	for (int i = 0; i < tempMatrix.x; i++)
	{
		for (int j = 0; j < tempMatrix.y; j++)
			if (tempMatrix.mat[i][j] == 1)
				cct_showint(j * 2, i, matrix.mat[i][j], COLOR_CYAN, COLOR_WHITE, 1);
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, max(matrix.x, matrix.y) + 1);
}

bool isValidAndSameColor(const Matrix& matrix, int x1, int y1, int x2, int y2)
{
	if (x1 < 0 || x1 >= matrix.x || y1 < 0 || y1 >= matrix.y || x2 < 0 || x2 >= matrix.x || y2 < 0 || y2 >= matrix.y)
	{
		return false;
	}
	return matrix.mat[x1][y1] == matrix.mat[x2][y2];
}

bool checkSwap(Matrix& matrix, int x, int y, int dx, int dy)
{
	swap(matrix.mat[x][y], matrix.mat[x + dx][y + dy]);

	bool horizontalMatch = (y >= 2 && matrix.mat[x][y] == matrix.mat[x][y - 1] && matrix.mat[x][y] == matrix.mat[x][y - 2]) ||
		(y <= 6 && matrix.mat[x][y] == matrix.mat[x][y + 1] && matrix.mat[x][y] == matrix.mat[x][y + 2]) ||
		(y >= 1 && y <= 7 && matrix.mat[x][y] == matrix.mat[x][y - 1] && matrix.mat[x][y] == matrix.mat[x][y + 1]);
	bool verticalMatch = (x >= 2 && matrix.mat[x][y] == matrix.mat[x - 1][y] && matrix.mat[x][y] == matrix.mat[x - 2][y]) ||
		(x <= 6 && matrix.mat[x][y] == matrix.mat[x + 1][y] && matrix.mat[x][y] == matrix.mat[x + 2][y]) ||
		(x >= 1 && x <= 7 && matrix.mat[x][y] == matrix.mat[x - 1][y] && matrix.mat[x][y] == matrix.mat[x + 1][y]);

	swap(matrix.mat[x][y], matrix.mat[x + dx][y + dy]);

	return horizontalMatch || verticalMatch;
}

bool markPotentialEliminations(const Matrix& matrix, Matrix& tempMatrix)
{
	for (int i = 0; i < matrix.x; i++)
	{
		for (int j = 0; j < matrix.y; j++)
			tempMatrix.mat[i][j] = 0;
	}

	int directions[4][2] = { {1, 0},{0, 1},{-1, 0},{0, -1} };
	bool foundElimination = false;
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			// 检查四个方向
			for (auto& dir : directions)
			{
				int newX = i + dir[0];
				int newY = j + dir[1];
				// 确保新位置在矩阵内
				if (newX >= 0 && newX < matrix.x && newY >= 0 && newY < matrix.y && matrix.mat[i][j] != 0)
				{
					auto tempSwapMat = matrix;
					if (checkSwap(tempSwapMat, i, j, dir[0], dir[1]))
					{
						// 标记可消除提示项
						tempMatrix.mat[i][j] = 1;
						tempMatrix.mat[newX][newY] = 1;
						foundElimination = true;
					}
				}
			}
		}
	}
	return foundElimination;
}