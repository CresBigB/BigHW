/* 班级 学号 姓名 */
#include "../90-01-b1/90-01-b1-hanoi.h"
extern int diskNums[3];
extern int disks[3][10];
/* ----------------------------------------------------------------------------------

	 本文件功能：
	1、放被 hanoi_main.cpp 调用的菜单函数，要求显示各菜单项，读入正确的选项后返回

	 本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、不允许定义静态全局变量（全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */
void mainMenu()
{
	while (true)
	{
		cct_cls();
		cout << "---------------------------------\n"
			"1.基本解\n"
			"2.基本解(步数记录)\n"
			"3.内部数组显示(横向)\n"
			"4.内部数组显示(纵向 + 横向)\n"
			"5.图形解 - 预备 - 画三个圆柱\n"
			"6.图形解 - 预备 - 在起始柱上画n个盘子\n"
			"7.图形解 - 预备 - 第一次移动\n"
			"8.图形解 - 自动移动版本\n"
			"9.图形解 - 游戏版\n"
			"0.退出\n"
			"----------------------------------\n"
			"[请选择:]";
		int ch;
		cin >> ch;
		switch (ch)
		{
		case 1:
			part_1();
			break;
		case 2:
			part_2();
			break;
		case 3:
			part_3();
			break;
		case 4:
			part_4();
			break;
		case 5:
			cct_cls();
			drawPillar();
			system("pause");
			break;
		case 6:
			part_6();
			break;
		case 7:
			part_7();
			break;
		case 8:
			part_8();
			break;
		case 9:
			part_9();
			break;
		case 0:
			return;
		default:
			cout << "无效选项\n";
		}
	}
}
void part_1()
{
	int disks_num = inputNum();
	char start = inputStart();
	char end = inputEnd(start);
	initialize_pillars(disks_num, start);
	char aux_pillar = get_aux_pillar(start, end);
	hanoi(disks_num, start, end, aux_pillar, 3);
	system("pause");
}
void part_2()
{
	int disks_num = inputNum();
	char start = inputStart();
	char end = inputEnd(start);
	initialize_pillars(disks_num, start);
	char aux_pillar = get_aux_pillar(start, end);
	hanoi(disks_num, start, end, aux_pillar, 4);
	system("pause");
}
void part_3()
{
	int disks_num = inputNum();
	char start = inputStart();
	char end = inputEnd(start);
	initialize_pillars(disks_num, start);
	char aux_pillar = get_aux_pillar(start, end);
	hanoi(disks_num, start, end, aux_pillar, 1);
	system("pause");
}
void part_4()
{
	int disks_num = inputNum();
	char start = inputStart();
	char end = inputEnd(start);
	initialize_pillars(disks_num, start);
	char aux_pillar = get_aux_pillar(start, end);
	setDelay();
	cct_cls();
	hanoi(disks_num, start, end, aux_pillar, 2);
	system("pause");
}
void part_6()
{
	int disks_num = inputNum();
	char start = inputStart();
	char end = inputEnd(start);
	initialize_pillars(disks_num, start);
	char aux_pillar = get_aux_pillar(start, end);
	cct_cls();
	drawPillarWithDisk();
	system("pause");
}
void part_7()
{
	int disks_num = inputNum();
	char start = inputStart();
	char end = inputEnd(start);
	initialize_pillars(disks_num, start);
	char aux_pillar = get_aux_pillar(start, end);
	setDelay();
	cct_cls();
	drawPillarWithDisk();
	moveDisk(start, end);
	system("pause");
}
void part_8()
{
	int disks_num = inputNum();
	char start = inputStart();
	char end = inputEnd(start);
	initialize_pillars(disks_num, start);
	char aux_pillar = get_aux_pillar(start, end);
	setDelay();
	cct_cls();
	drawPillarWithDisk();
	hanoi(disks_num, start, end, aux_pillar, 99);
	system("pause");
}
void part_9()
{
	int disks_num = inputNum();
	char start = inputStart();
	char end = inputEnd(start);
	initialize_pillars(disks_num, start);
	char aux_pillar = get_aux_pillar(start, end);
	setDelay();
	cct_cls();
	drawPillarWithDisk();
	while (diskNums[end - 'A'] != disks_num)
	{
		cct_gotoxy(0, 36);
		cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) ：";
		char from, to;
		cin >> from >> to;
		if ((from >= 'a' && from <= 'c' || from >= 'A' && from <= 'C') && (to >= 'a' && to <= 'c' || to >= 'A' && to <= 'C'))
		{
			if (from >= 'a' && from <= 'c') from -= 32;
			if (to >= 'a' && to <= 'c') to -= 32;
			if (canMove(from, to))
			{
				moveDisk(from, to);
			}
			else
			{
				cct_gotoxy(0, 37);
				cout << "不能移动空柱子";
				Sleep(500);
				cct_gotoxy(0, 37);
				cout << "\t\t\t\t\t\t\t\t";
			}
			showArrayInfo(5, disks[from - 'A'][diskNums[from - 'A']], from, to);
		}
		else
		{
			cct_gotoxy(0, 36);
			cout << "无效的选择";
		}
	}
	system("pause");
}