#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;
#define NORMAL_STATE       1
#define RELATED_STATE      2
#define SELECTED_STATE     3
#define BONUS              10

struct elem
{
	int key;
	int state;//1:原始状态;2:关联状态（选中区域）;3:选中状态（1次回车后）
};

typedef struct a
{
	int mat[9][9];
	int x = 0;	//行数
	int y = 0;	//列数
}Matrix;

void setMatrixSize(Matrix& matrix)
{
	while (true)
	{
		cout << "请输入行数（5-9）：";
		cin >> matrix.x;
		cout << "请输入列数（5-9）：";
		cin >> matrix.y;
		if (matrix.x >= 5 && matrix.y >= 5 && matrix.x <= 9 && matrix.y <= 9)
			return;
		else
			cout << "错误的输入\n";
	}
}

void fillEmptySlots(CONSOLE_GRAPHICS_INFO& CGI, Matrix& matrix)
{
	const BLOCK_DISPLAY_INFO bdi_normal[] =
	{
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
	{1, COLOR_HBLACK, -1, "〇"},
	{2, COLOR_YELLOW, -1, "〇"},
	{3, COLOR_HGREEN, -1, "〇"},
	{4, COLOR_HCYAN, -1, "〇"},
	{5, COLOR_HRED, -1, "〇"},
	{6, COLOR_HPINK, -1, "〇"},
	{7, COLOR_HYELLOW, -1, "〇"},
	{8, COLOR_CYAN, -1, "〇"},
	{9, COLOR_WHITE, -1, "〇"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};

	if (matrix.x < 5) matrix.x = 5;
	if (matrix.x > 9) matrix.x = 9;
	if (matrix.y < 5) matrix.y = 5;
	if (matrix.y > 9) matrix.y = 9;
	srand(time(0));
	for (int i = 0; i < matrix.x; i++)
	{
		for (int j = 0; j < matrix.y; j++)
			if (matrix.mat[i][j] == 0)
			{
				matrix.mat[i][j] = rand() % 9 + 1;
				gmw_draw_block(&CGI, i, j, matrix.mat[i][j], bdi_normal);
			}
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

int eliminateElimination(const CONSOLE_GRAPHICS_INFO& CGI, Matrix& matrix, Matrix& tempMatrix)
{
	const BLOCK_DISPLAY_INFO bdi_normal[] =
	{
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
	{1, COLOR_HBLACK, -1, "〇"},
	{2, COLOR_YELLOW, -1, "〇"},
	{3, COLOR_HGREEN, -1, "〇"},
	{4, COLOR_HCYAN, -1, "〇"},
	{5, COLOR_HRED, -1, "〇"},
	{6, COLOR_HPINK, -1, "〇"},
	{7, COLOR_HYELLOW, -1, "〇"},
	{8, COLOR_CYAN, -1, "〇"},
	{9, COLOR_WHITE, -1, "〇"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};

	int cnt = 0;
	for (int i = 0; i < matrix.x; ++i)
	{
		for (int j = 0; j < matrix.y; ++j)
		{
			if (tempMatrix.mat[i][j])
			{
				gmw_draw_block(&CGI, i, j, BDI_VALUE_BLANK, bdi_normal);
				++cnt;
				tempMatrix.mat[i][j] = 0;
				matrix.mat[i][j] = 0;
			}
		}
	}
	return cnt;
}

void fallTheBall(CONSOLE_GRAPHICS_INFO& CGI, Matrix& matrix)
{
	const BLOCK_DISPLAY_INFO bdi_normal[] =
	{
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
	{1, COLOR_HBLACK, -1, "〇"},
	{2, COLOR_YELLOW, -1, "〇"},
	{3, COLOR_HGREEN, -1, "〇"},
	{4, COLOR_HCYAN, -1, "〇"},
	{5, COLOR_HRED, -1, "〇"},
	{6, COLOR_HPINK, -1, "〇"},
	{7, COLOR_HYELLOW, -1, "〇"},
	{8, COLOR_CYAN, -1, "〇"},
	{9, COLOR_WHITE, -1, "〇"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};

	for (int t = 0; t < matrix.x; ++t)
	{
		for (int i = 0; i < matrix.x - 1; i++)
		{
			for (int j = 0; j < matrix.y; j++)
				if (matrix.mat[i + 1][j] == 0 && matrix.mat[i][j] != 0)
				{
					gmw_move_block(&CGI, i, j, matrix.mat[i][j], 0, bdi_normal, UP_TO_DOWN, 1);
					swap(matrix.mat[i][j], matrix.mat[i + 1][j]);
					gmw_draw_block(&CGI, i, j, BDI_VALUE_BLANK, bdi_normal);
				}
		}
	}
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

int graph_markPotentialEliminations(CONSOLE_GRAPHICS_INFO& CGI, const Matrix& matrix, Matrix& tempMatrix)
{
	/* 定义1-9的数字用何种形式显示在界面上（可消除提示状态） */
	const BLOCK_DISPLAY_INFO bdi_prompt[] =
	{
	{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
	{1, COLOR_HBLACK, -1, "◎"},
	{2, COLOR_YELLOW, -1, "◎"},
	{3, COLOR_HGREEN, -1, "◎"},
	{4, COLOR_HCYAN, -1, "◎"},
	{5, COLOR_HRED, -1, "◎"},
	{6, COLOR_HPINK, -1, "◎"},
	{7, COLOR_HYELLOW, -1, "◎"},
	{8, COLOR_CYAN, -1, "◎"},
	{9, COLOR_WHITE, -1, "◎"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};

	markPotentialEliminations(matrix, tempMatrix);
	int num = 0;
	for (int i = 0; i < tempMatrix.x; i++)
	{
		for (int j = 0; j < tempMatrix.y; j++)
			if (tempMatrix.mat[i][j] == 1)
			{
				gmw_draw_block(&CGI, i, j, matrix.mat[i][j], bdi_prompt);
				++num;
			}
	}
	return num;
}

void useMouseSwapElement(CONSOLE_GRAPHICS_INFO& CGI, Matrix& matrix)
{
	int x1 = -1, y1 = -1; // 记录第一次点击的坐标
	int x2, y2; // 记录第二次点击的坐标
	int MAction;
	int keycode1;
	int keycode2;

	const BLOCK_DISPLAY_INFO bdi_normal[] =
	{
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
	{1, COLOR_HBLACK, -1, "〇"},
	{2, COLOR_YELLOW, -1, "〇"},
	{3, COLOR_HGREEN, -1, "〇"},
	{4, COLOR_HCYAN, -1, "〇"},
	{5, COLOR_HRED, -1, "〇"},
	{6, COLOR_HPINK, -1, "〇"},
	{7, COLOR_HYELLOW, -1, "〇"},
	{8, COLOR_CYAN, -1, "〇"},
	{9, COLOR_WHITE, -1, "〇"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};

	while (true)
	{
		// 读取鼠标输入
		gmw_read_keyboard_and_mouse(&CGI, MAction, x1, y1, keycode1, keycode2, true);
		char temp[256] = { 0 };
		if (MAction == MOUSE_LEFT_BUTTON_CLICK)
		{
			snprintf(temp, 255, "鼠标位置X:%d,Y:%d", x1, y1);
			gmw_status_line(&CGI, LOWER_STATUS_LINE, temp);
			// 第一次鼠标点击位置有效
			while (true)
			{
				// 读取第二次鼠标输入
				gmw_read_keyboard_and_mouse(&CGI, MAction, x2, y2, keycode1, keycode2, true);
				if (MAction == MOUSE_LEFT_BUTTON_CLICK) break; // 等待第二次点击
			}

			// 检查两次点击的坐标是否相邻
			if ((x2 == x1 && abs(y2 - y1) == 1) || (y2 == y1 && abs(x2 - x1) == 1))
			{
				snprintf(temp, 256, "鼠标位置X:%d,Y:%d", x1, y1);
				gmw_status_line(&CGI, LOWER_STATUS_LINE, temp);
				// 交换矩阵中的两个相邻元素
				std::swap(matrix.mat[x1][y1], matrix.mat[x2][y2]);

				// 更新显示
				gmw_draw_block(&CGI, x1, y1, matrix.mat[x1][y1], bdi_normal);
				gmw_draw_block(&CGI, x2, y2, matrix.mat[x2][y2], bdi_normal);

				break; // 完成交换后退出
			}
			else
			{
				// 提示用户两次选择的元素不相邻
				gmw_status_line(&CGI, LOWER_STATUS_LINE, "选择的元素不相邻，请重新选择。");
				x1 = -1; // 重置第一次点击
			}
		}
	}
}

void setNull(Matrix& matrix)
{
	for (int i = 0; i < matrix.x; ++i)
		for (int j = 0; j < matrix.y; ++j)
			matrix.mat[i][j] = 0;
}

void start_magic_ball(CONSOLE_GRAPHICS_INFO* pMagicBall_CGI)
{
	const char* gname = "MagicBall";
	cct_cls();

	/* 预置一个数组 */
	Matrix matrix = { 0 };
	Matrix teMatrix = { 0 };

	setMatrixSize(matrix);
	teMatrix.x = matrix.x;
	teMatrix.y = matrix.y;

	/* 定义1-9的数字用何种形式显示在界面上（正常状态） */
	const BLOCK_DISPLAY_INFO bdi_normal[] =
	{
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
	{1, COLOR_HBLACK, -1, "〇"},
	{2, COLOR_YELLOW, -1, "〇"},
	{3, COLOR_HGREEN, -1, "〇"},
	{4, COLOR_HCYAN, -1, "〇"},
	{5, COLOR_HRED, -1, "〇"},
	{6, COLOR_HPINK, -1, "〇"},
	{7, COLOR_HYELLOW, -1, "〇"},
	{8, COLOR_CYAN, -1, "〇"},
	{9, COLOR_WHITE, -1, "〇"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	/* 定义1-9的数字用何种形式显示在界面上（选中状态） */
	const BLOCK_DISPLAY_INFO bdi_selected[] =
	{
	{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
	{1, COLOR_HBLACK, -1, "●"},
	{2, COLOR_YELLOW, -1, "●"},
	{3, COLOR_HGREEN, -1, "●"},
	{4, COLOR_HCYAN, -1, "●"},
	{5, COLOR_HRED, -1, "●"},
	{6, COLOR_HPINK, -1, "●"},
	{7, COLOR_HYELLOW, -1, "●"},
	{8, COLOR_CYAN, -1, "●"},
	{9, COLOR_WHITE, -1, "●"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	/* 定义1-9的数字用何种形式显示在界面上（可消除提示状态） */
	const BLOCK_DISPLAY_INFO bdi_prompt[] =
	{
	{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
	{1, COLOR_HBLACK, -1, "◎"},
	{2, COLOR_YELLOW, -1, "◎"},
	{3, COLOR_HGREEN, -1, "◎"},
	{4, COLOR_HCYAN, -1, "◎"},
	{5, COLOR_HRED, -1, "◎"},
	{6, COLOR_HPINK, -1, "◎"},
	{7, COLOR_HYELLOW, -1, "◎"},
	{8, COLOR_CYAN, -1, "◎"},
	{9, COLOR_WHITE, -1, "◎"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	/* 定义1-9的数字用何种形式显示在界面上（爆炸/消除状态） */
	const BLOCK_DISPLAY_INFO bdi_exploded[] =
	{
	{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
	{1, COLOR_HBLACK, -1, "¤"},
	{2, COLOR_YELLOW, -1, "¤"},
	{3, COLOR_HGREEN, -1, "¤"},
	{4, COLOR_HCYAN, -1, "¤"},
	{5, COLOR_HRED, -1, "¤"},
	{6, COLOR_HPINK, -1, "¤"},
	{7, COLOR_HYELLOW, -1, "¤"},
	{8, COLOR_CYAN, -1, "¤"},
	{9, COLOR_WHITE, -1, "¤"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	char temp[256];

	gmw_set_font(pMagicBall_CGI, "新宋体", 24);
	gmw_set_frame_style(pMagicBall_CGI, 6, 3);
	/* 按row/col的值重设游戏主区域行列 */
	gmw_set_rowcol(pMagicBall_CGI, matrix.x, matrix.y);
	/* 显示框架 */
	gmw_draw_frame(pMagicBall_CGI);
	///* 上状态栏显示内容 */
	//sprintf(temp, "%s-窗口大小：%d行 %d列", gname, pMagicBall_CGI->lines, pMagicBall_CGI->cols);
	//gmw_status_line(pMagicBall_CGI, TOP_STATUS_LINE, temp);
	///* 下状态栏显示内容 */
	gmw_status_line(pMagicBall_CGI, LOWER_STATUS_LINE, "输入End返回", "测试结束，");
	fillEmptySlots(*pMagicBall_CGI, matrix);
	//for (int i = 0; i < matrix.x; i++)
	//{
	//	for (int j = 0; j < matrix.y; j++)
	//		gmw_draw_block(pMagicBall_CGI, i, j, matrix.mat[i][j], bdi_normal);
	//}

	int MAction = 0;
	int MRow = 0;
	int MCol = 0;
	int keycode1 = 0;
	int keycode2 = 0;
	int cnt = 0;

	while (true)
	{
		findElimination(matrix, teMatrix);
		int c = eliminateElimination(*pMagicBall_CGI, matrix, teMatrix);
		if (c == 0) break;
		cnt += c;
		fallTheBall(*pMagicBall_CGI, matrix);
		fillEmptySlots(*pMagicBall_CGI, matrix);
	}

	while (true)
	{
		findElimination(matrix, teMatrix);
		while (true)
		{
			findElimination(matrix, teMatrix);
			int c = eliminateElimination(*pMagicBall_CGI, matrix, teMatrix);
			if (c == 0) break;
			cnt += c;
			fallTheBall(*pMagicBall_CGI, matrix);
			fillEmptySlots(*pMagicBall_CGI, matrix);
		}
		fallTheBall(*pMagicBall_CGI, matrix);
		fillEmptySlots(*pMagicBall_CGI, matrix);
		auto p = graph_markPotentialEliminations(*pMagicBall_CGI, matrix, teMatrix);
		if (p == 0) break;
		useMouseSwapElement(*pMagicBall_CGI, matrix);

		setNull(teMatrix);
	}
}

int main()
{
	srand(unsigned int(time(0)));

	CONSOLE_GRAPHICS_INFO MagicBall_CGI; //声明一个CGI变量

	gmw_init(&MagicBall_CGI);
	cct_enable_mouse();

	gmw_set_ext_rowcol(&MagicBall_CGI, 0, 0, 0, 30);						//右边留30列
	gmw_set_color(&MagicBall_CGI, COLOR_BLACK, COLOR_HWHITE);			//整个窗口颜色
	gmw_set_font(&MagicBall_CGI, "新宋体", 24);							//字体
	//	gmw_set_frame_style(&MagicBall_CGI);									//游戏主区域风格：每个色块宽2高1，有分隔线
	gmw_set_frame_default_linetype(&MagicBall_CGI, 4);					//游戏主区域线型：横单竖双
	gmw_set_frame_color(&MagicBall_CGI, COLOR_HWHITE, COLOR_BLACK);		//游戏主区域颜色
	gmw_set_rowno_switch(&MagicBall_CGI, true);							//显示行号
	gmw_set_colno_switch(&MagicBall_CGI, true);							//不显示列标
	gmw_set_status_line_switch(&MagicBall_CGI, TOP_STATUS_LINE, false);	//不需要上状态栏
	gmw_set_status_line_switch(&MagicBall_CGI, LOWER_STATUS_LINE, true);	//需要下状态栏	int score = 0;
	gmw_set_delay(&MagicBall_CGI, DELAY_OF_BLOCK_MOVED, 10);
	gmw_set_delay(&MagicBall_CGI, DELAY_OF_DRAW_BLOCK, 0);
	while (true)
	{
		start_magic_ball(&MagicBall_CGI);

		while (_getche() != '\r');
	}
}