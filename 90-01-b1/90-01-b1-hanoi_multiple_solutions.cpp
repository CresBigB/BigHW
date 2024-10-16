/* 班级 学号 姓名 */
#include "90-01-b1-hanoi.h"
/* ----------------------------------------------------------------------------------

	 本文件功能：
	1、存放被 hanoi_main.cpp 中根据菜单返回值调用的各菜单项对应的执行函数

	 本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */

int disks[3][10];
int diskNums[3];
int total_moves = 0;							// 总移动步数
int timeDelay = 50;								//时延设置

void stop()
{
	if (timeDelay == 0)
		system("pause");
	else Sleep(timeDelay);
}

void setDelay()
{
	while (true)
	{
		cout << "请输入移动速度(0-5：0-按回车单步演示 1-延时最长 5-延时最短):";
		int ch;
		cin >> ch;
		switch (ch)
		{
		case 0:timeDelay = 0; break;
		case 1:timeDelay = 150; break;
		case 2:timeDelay = 90; break;
		case 3:timeDelay = 45; break;
		case 4:timeDelay = 15; break;
		case 5:timeDelay = 5; break;
		default: continue;
		}
		if (ch >= 0 && ch <= 5) break;
	}
}

/**
 * @brief 获取辅助柱
 *
 * @param from 起始柱
 * @param to 目标柱
 * @return char 辅助柱
 */
char get_aux_pillar(char from, char to)
{
	if ((from == 'A' && to == 'B') || (from == 'B' && to == 'A'))
		return 'C';
	else if ((from == 'A' && to == 'C') || (from == 'C' && to == 'A'))
		return 'B';
	else
		return 'A';
}

/**
 * @brief 移动圆盘并输出移动信息
 *
 * @param from 起始柱
 * @param to 目标柱
 */
void move_disk(char from, char to, int model)
{
	int disk = 0;

	// 从起始柱移除顶部圆盘
	if (from >= 'A' && from <= 'C')
	{
		disk = disks[from - 'A'][diskNums[from - 'A'] - 1];
		disks[from - 'A'][diskNums[from - 'A'] - 1] = 0;
		--diskNums[from - 'A'];
	}

	// 将圆盘添加到目标柱
	if (to >= 'A' && to <= 'C')
	{
		disks[to - 'A'][diskNums[to - 'A']] = disk;
		++diskNums[to - 'A'];
	}
	++total_moves;
	showArrayInfo(model, disk, from, to);

}

void showArrayInfo(int model, int disk, char from, char to)
{
	switch (model)
	{
	case 1:
	{
		// 输出移动信息
		cout << "第" << total_moves << "步，" << "移动圆盘 " << disk << " 从 " << from << " 到 " << to;

		for (int i = 0; i < 3; ++i)
		{
			cout << "\t" << static_cast<char>('A' + i) << ":";
			for (int j = 0; j < diskNums[i]; ++j)
				cout << disks[i][j] << " ";
			for (int j = 0; j < 10 - diskNums[i]; ++j)
				cout << "  ";
		}
		cout << "\n";
		break;
	}
	case 2:
	{
		cct_gotoxy(X, Y);
		for (int i = 0; i < 19; ++i)
			cout << "=";
		for (int i = 1; i <= 3; ++i)
		{
			cct_gotoxy((X + 2) * i, Y + 1);
			cout << static_cast<char>('A' - 1 + i);
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < diskNums[i]; ++j)
			{
				cct_gotoxy((X + 2) * (i + 1), Y - 1 - j);
				cout << disks[i][j];
			}
			for (int j = diskNums[i]; j < 10; ++j)
			{
				cct_gotoxy((X + 2) * (i + 1), Y - 1 - j);
				cout << "  ";
			}
		}
		cct_gotoxy(0, Y + 3);
		cout << "第" << total_moves << "步，" << "移动圆盘 " << disk << " 从 " << from << " 到 " << to << "\n";
		stop();
		break;
	}
	case 3:
		cout << "移动圆盘 " << disk << " 从 " << from << " 到 " << to << "\n";
		break;
	case 4:
		cout << "第" << total_moves << "步，" << "移动圆盘 " << disk << " 从 " << from << " 到 " << to << "\n";
		break;
	case 5:
		cct_gotoxy(X, 32);
		for (int i = 0; i < 19; ++i)
			cout << "=";
		for (int i = 1; i <= 3; ++i)
		{
			cct_gotoxy((X + 2) * i, 32 + 1);
			cout << static_cast<char>('A' - 1 + i);
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < diskNums[i]; ++j)
			{
				cct_gotoxy((X + 2) * (i + 1), 32 - 1 - j);
				cout << disks[i][j];
			}
			for (int j = diskNums[i]; j < 10; ++j)
			{
				cct_gotoxy((X + 2) * (i + 1), 32 - 1 - j);
				cout << "  ";
			}
		}
		cct_gotoxy(0, 32 + 3);
		cout << "第" << total_moves << "步，" << "移动圆盘 " << disk << " 从 " << from << " 到 " << to << "\n";
		cct_gotoxy(0, 32 + 4);
		break;
	default:return;
	}
}

/**
 * @brief 递归实现汉诺塔算法
 *
 * @param n 圆盘数量
 * @param from 起始柱
 * @param to 目标柱
 * @param aux 辅助柱
 */
void hanoi(int n, char from, char to, char aux, int model)
{
	if (n == 1)
	{
		if (model == 99)
			moveDisk(from, to);
		else
			move_disk(from, to, model);
	}
	else
	{
		hanoi(n - 1, from, aux, to, model);
		if (model == 99)
			moveDisk(from, to);
		else
			move_disk(from, to, model);
		hanoi(n - 1, aux, to, from, model);
	}
}

/**
 * @brief 初始化起始柱上的圆盘
 *
 * @param num_disks 圆盘数量
 * @param start_pillar 起始柱
 */
void initialize_pillars(int num_disks, char start_pillar)
{
	timeDelay = 0;
	total_moves = 0;
	for (int i = 0; i < 3; ++i)
	{
		diskNums[i] = 0;
		for (int j = 0; j < 10; ++j)
			disks[i][j] = 0;
	}

	for (int i = 0; i < num_disks; i++)
	{
		int disk_size = num_disks - i; // 大盘在下，小盘在上
		disks[start_pillar - 'A'][i] = disk_size;
		++diskNums[start_pillar - 'A'];
	}
}

constexpr int width = 25;	//底部宽度
constexpr int height = 11;	//柱子的高
constexpr int x = 0;		//底部的X坐标
constexpr int y = 20;		//底部的Y坐标

bool canMove(char from, char to)
{
	if (disks[from - 'A'][diskNums[from - 'A'] - 1] &&
		(disks[to - 'A'][diskNums[to - 'A'] - 1] > disks[from - 'A'][diskNums[from - 'A'] - 1]
			|| disks[to - 'A'][diskNums[to - 'A'] - 1] == 0))
		return true;
	return false;
}


void drawPillar()
{
	for (int idx = 0; idx < 3; ++idx)
	{
		int begin = x + width * idx;
		begin += 5 * idx;
		for (int i = 0; i < width; ++i)
		{
			cct_showstr(begin + i, y, " ", COLOR_HYELLOW, COLOR_BLACK);
		}
		for (int i = 1; i <= height; ++i)
		{
			cct_showstr(begin + width / 2, y - i, " ", COLOR_HYELLOW, COLOR_BLACK);
		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 22);
}

void drawPillarWithDisk()
{
	cct_cls();
	for (int idx = 0; idx < 3; ++idx)
	{
		int begin = x + width * idx;
		begin += 5 * idx;
		for (int i = 0; i < width; ++i)
		{
			cct_showstr(begin + i, y, " ", COLOR_HYELLOW, COLOR_BLACK);
		}
		for (int i = 1; i <= height; ++i)
		{
			cct_showstr(begin + width / 2, y - i, " ", COLOR_HYELLOW, COLOR_BLACK);
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		int begin = x + width * i + i * 5;
		for (int j = 0; j < diskNums[i]; ++j)
		{
			int diskWidth = disks[i][j] * 2 + 1;
			for (int k = 0; k < diskWidth; ++k)
				cct_showstr(begin + width / 2 - diskWidth / 2 + k, y - 1 - j, " ", disks[i][j], COLOR_BLACK);
		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 22);
}

void moveDisk(char from, char to)
{
	++total_moves;
	int disk = 0;
	int from_idx = from - 'A';
	int to_idx = to - 'A';
	int disk_idx = diskNums[from_idx] - 1;

	// 从起始柱移除顶部圆盘
	if (from >= 'A' && from <= 'C')
	{
		disk = disks[from_idx][disk_idx];
		disks[from_idx][disk_idx] = 0;
		--diskNums[from_idx];
	}

	// 将圆盘添加到目标柱
	if (to >= 'A' && to <= 'C')
	{
		disks[to_idx][diskNums[to_idx]] = disk;
		++diskNums[to_idx];
	}

	int begin = x + width * from_idx + from_idx * 5;	//from的底座的X坐标
	int end = x + width * to_idx + to_idx * 5;			//to的底座的X坐标
	int diskWidth = disk * 2 + 1;						//移动的disk的宽度
	int disk_y = y - disk_idx - 1;
	int disk_x = begin + width / 2 - diskWidth / 2;

	//向上移动
	for (int i = disk_y; i > 2; --i)
	{
		int begin = x + width * from_idx;
		for (int j = 0; j < diskWidth; ++j)
			cct_showstr(disk_x + j, i - 1, " ", disk, COLOR_BLACK);
		for (int j = 0; j < diskWidth; ++j)
			cct_showstr(disk_x + j, i, " ", COLOR_BLACK, COLOR_BLACK);
		if (i >= y - height)
		{
			cct_showstr(begin + width / 2 + 5 * from_idx, i, " ", COLOR_HYELLOW, COLOR_BLACK);
		}
		cct_gotoxy(x, y + 2);
		stop();
	}

	disk_y = 2;
	//左右移动
	while (true)
	{
		if (begin < end)
		{
			for (int j = 0; j < diskWidth; ++j)
				cct_showstr(disk_x + j, disk_y, " ", disk, COLOR_BLACK);
			cct_showstr(disk_x, disk_y, " ", COLOR_BLACK, COLOR_BLACK);

			++disk_x;
		}

		if (begin > end)
		{
			for (int j = 0; j <= diskWidth; ++j)
				cct_showstr(disk_x + j, disk_y, " ", COLOR_BLACK, COLOR_BLACK);
			for (int j = 0; j < diskWidth; ++j)
				cct_showstr(disk_x - j, disk_y, " ", disk, COLOR_BLACK);

			--disk_x;
		}

		if (disk_x == end + width / 2 - diskWidth / 2) break;

		cct_setcolor(COLOR_BLACK, COLOR_WHITE);
		cct_gotoxy(x, y + 2);
		stop();
	}
	for (int i = 0; i < 300; i++)
		cct_showstr(i, 2, " ", COLOR_BLACK, COLOR_BLACK);
	//向下移动
	for (int i = disk_y; i <= y - diskNums[to_idx] - 1; ++i)
	{
		//int begin = x + width * from_idx;
		for (int j = 0; j < diskWidth; ++j)
			cct_showstr(disk_x + j, i, " ", COLOR_BLACK, COLOR_BLACK);
		for (int j = 0; j < diskWidth; ++j)
			cct_showstr(disk_x + j, i + 1, " ", disk, COLOR_BLACK);
		if (i <= y - 1 && i >= y - height)
		{
			cct_showstr(end + width / 2, i, " ", COLOR_HYELLOW, COLOR_BLACK);
		}
		cct_gotoxy(x, y + 2);
		stop();
		//stop();
	}

	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}

void graph_hanoi(int n, char from, char to, char aux)
{
	if (n == 1)
	{
		moveDisk(from, to);
	}
	else
	{
		graph_hanoi(n - 1, from, aux, to);
		moveDisk(from, to);
		graph_hanoi(n - 1, aux, to, from);
	}
}

int inputNum()
{
	int num_disks;
	do
	{
		cout << "请输入汉诺塔的层数 (1-10): ";
		cin >> num_disks;
		if (num_disks < 1 || num_disks > 10)
		{
			cout << "层数必须在1到10之间，请重新输入。\n";
		}
	} while (num_disks < 1 || num_disks > 10);
	return num_disks;
}

char inputStart()
{
	char start_pillar;
	do
	{
		cout << "请选择起始柱 (A, B, C): ";
		cin >> start_pillar;
		if (start_pillar >= 'A' && start_pillar <= 'C' || start_pillar >= 'a' || start_pillar <= 'c')
			break;
		else
			cout << "无效的柱，请输入A, B或C。\n";

	} while (true);
	if (start_pillar >= 'a' && start_pillar <= 'z')
		return start_pillar - 32;
	return start_pillar;
}

char inputEnd(char start)
{
	char target_pillar;
	do
	{
		cout << "请选择目标柱 (A, B, C), 不能与起始柱相同: ";
		cin >> target_pillar;
		if ((target_pillar >= 'a' && target_pillar <= 'c' || target_pillar >= 'A' && target_pillar <= 'C') && target_pillar != start)
			break;
		else
			cout << "无效的柱或与起始柱相同，请重新输入。\n";
	} while (true);
	if (target_pillar >= 'a' && target_pillar <= 'z')
		return target_pillar - 32;
	return target_pillar;
}
