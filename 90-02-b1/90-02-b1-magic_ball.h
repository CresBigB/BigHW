#pragma once
#include <iostream>
#include "../include/cmd_console_tools.h"
#include <conio.h>
using namespace std;

// ★ ✰ ☆ ✩ ✫ ✬ ✭ ✮ ✡


typedef struct b
{
	int mouse_x = 0;
	int mouse_y = 0;
	int mouse_action = 0;
	int keyCode_1 = 0;
	int keyCode_2 = 0;
}MouseAction;

typedef struct a
{
	int mat[10][10];
	int x = 0;	//行数
	int y = 0;	//列数
	int emptyColumn = 0;
}Matrix;

/*工具函数*/
void keyHit(MouseAction& action);
int countItem(const Matrix& matrix);
void setMatrixSize(Matrix& matrix);														//设置矩阵大小
int DFS(int x, int y, int color, const Matrix& matrix, Matrix& tempMatrix);				//DFS查找可消除项
void fillEmptySlots(Matrix& matrix);													//填充空白区域
int findElimination(int x, int y, const Matrix& matrix, Matrix& tempMatrix);			//查找可消除项
int  eliminateElements(Matrix& matrix, Matrix& tempMatrix);								//清除可清除项
void fallTheBall(Matrix& matrix);														//使球下落
void setEmpty(Matrix& matrix);															//置空矩阵
bool isGameOver(const Matrix& matrix);													//判断是否游戏结束
MouseAction getMouseAction();															//获取鼠标事件信息
int  showMouseLocation(const Matrix& matrix, int x, int y);								//显示鼠标位置
void moveColumn(int idx, Matrix& matrix);												//将下标为idx的列向左平移
int getEmptyColumnIdx(const Matrix& matrix);											//获取矩阵中为空的列的下标
/*数组版界面函数*/
void showMatrix(const Matrix& matrix);													//显示数组界面
void setColorForElimination(const Matrix& matrix, const Matrix& tempMatrix);			//着色可消除项
bool isValidAndSameColor(const Matrix& matrix, int x1, int y1, int x2, int y2);			//判断颜色是否相同且在边界内
bool markPotentialEliminations(const Matrix& matrix, Matrix& tempMatrix);				//标记可消除提示项
/*伪图形界面函数*/
void drawGraph_No_Border(const Matrix& matrix);											//显示没有分割线的界面
void drawGraph_With_Border(const Matrix& matrix);										//显示有分割线的界面
void graph_setColorForElimination(const Matrix& matrix, const Matrix& tempMatrix);		//着色可消除项
int useMouseSelectElement(Matrix& matrix, Matrix& tempMatrix, MouseAction& action);		//使用鼠标交换元素
void graph_fallTheBall(Matrix& matrix);													//使球下落
void graph_fillEmptySlots(Matrix& matrix);												//伪图形界面填充空位
int graph_markPotentialEliminations(const Matrix& matrix, Matrix& tempMatrix);			//伪图形界面标记消除提示项
int graph_eliminateElements(Matrix& matrix, Matrix& tempMatrix);						//清除可清除项
void graph_moveColumn(int idx, Matrix& matrix);
/*菜单类函数*/
void mainMenu();																		//主界面菜单
void part_1();																			//第1题
void part_2();																			//第2题
void part_3();																			//第3题
void part_4();																			//第4题
void part_5();																			//第5题
void part_6();																			//第6题
void part_7();																			//第7题
void part_8();																			//第8题
void part_9();																			//第9题