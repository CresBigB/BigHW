/* 班级 学号 姓名 */
#pragma once
#include "../include/cmd_console_tools.h"
#include <Windows.h>
#include <iostream>
#define X 5
#define Y 12
using namespace std;

/* ------------------------------------------------------------------------------------------------------

	 本文件功能：
	1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp 能相互访问函数的函数声明
	2、一个以上的cpp中用到的宏定义（#define）或全局只读（const）变量，个数不限
	3、可以参考 cmd_console_tools.h 的写法（认真阅读并体会）
   ------------------------------------------------------------------------------------------------------ */

void stop();
void setDelay();
char get_aux_pillar(char from, char to);
void move_disk(char from, char to,int model);
void hanoi(int n, char from, char to, char aux, int model);
void initialize_pillars(int num_disks, char start_pillar);
void showArrayInfo(int model, int disk, char from, char to);
bool canMove(char from, char to);

int inputNum();
char inputStart();
char inputEnd(char start);

void drawPillar();
void drawPillarWithDisk();
void moveDisk(char from, char to);

void mainMenu();
void part_1();
void part_2();
void part_3();
void part_4();
void part_5();
void part_6();
void part_7();
void part_8();
void part_9();
void test();