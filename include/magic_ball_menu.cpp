#include "../90-01-b2/90-01-b2-magic_ball.h"

void mainMenu()
{
	while (true)
	{
		cct_setconsoleborder(100, 30, 20, 8);
		cct_setfontsize("新宋体", 26, 13);
		system("cls");
		cout << "1.内部数组，生成初始状态，寻找是否有初始可消除项\n";
		cout << "2.内部数组，消除初始可消除项后非0项下落并用0填充\n";
		cout << "3.内部数组，消除初始可消除项后查找消除提示\n";
		cout << "4.n*n的框架（无分隔线），显示初始状态\n";
		cout << "5.n*n的框架（有分隔线），显示初始状态\n";
		cout << "6.n*n的框架（有分隔线），显示初始状态及初始可消除项\n";
		cout << "7.n*的框架（有分隔线），消除初始可消除项后显示消除提示\n";
		cout << "8.cmd图形界面完整版（有分隔线，鼠标移动时显示坐标，右键退出）\n";
		cout << "9.cmd图形界面完整版\n";
		cout << "0.退出\n";
		cout << "请选择:";
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:part_1(); break;
		case 2:part_2(); break;
		case 3:part_3(); break;
		case 4:part_4(); break;
		case 5:part_5(); break;
		case 6:part_6(); break;
		case 7:part_7(); break;
		case 8:part_8(); break;
		case 9:part_9(); break;
		case 0:return;
		default:
			cout << "错误的选项\n";
		}
	}
}

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

void part_1()
{
	Matrix matrix{};
	Matrix tempMatrix{};
	setMatrixSize(matrix);
	tempMatrix.x = matrix.x;
	tempMatrix.y = matrix.y;
	fillEmptySlots(matrix);
	findElimination(matrix, tempMatrix);
	showMatrix(matrix);
	setColorForElimination(matrix, tempMatrix);
	system("pause");
}

void part_2()
{
	Matrix matrix{};
	Matrix tempMatrix{};
	setMatrixSize(matrix);
	system("cls");
	tempMatrix.x = matrix.x;
	tempMatrix.y = matrix.y;
	fillEmptySlots(matrix);
	findElimination(matrix, tempMatrix);
	showMatrix(matrix);
	setColorForElimination(matrix, tempMatrix);
	system("pause");
	eliminateElements(matrix, tempMatrix);
	showMatrix(matrix);
	system("pause");
	fallTheBall(matrix);
	showMatrix(matrix);
	system("pause");
	fillEmptySlots(matrix);
	showMatrix(matrix);
	system("pause");
}

void part_3()
{
	Matrix matrix{};
	Matrix tempMatrix{};
	setMatrixSize(matrix);
	system("cls");
	tempMatrix.x = matrix.x;
	tempMatrix.y = matrix.y;
	fillEmptySlots(matrix);
	findElimination(matrix, tempMatrix);
	showMatrix(matrix);
	setColorForElimination(matrix, tempMatrix);
	system("pause");
	eliminateElements(matrix, tempMatrix);
	showMatrix(matrix);
	system("pause");
	markPotentialEliminations(matrix, tempMatrix);
	setColorForElimination(matrix, tempMatrix);
	system("pause");
}

void part_4()
{
	Matrix matrix{};
	setMatrixSize(matrix);
	system("cls");
	fillEmptySlots(matrix);
	drawGraph_No_Border(matrix);
	system("pause");
}

void part_5()
{
	Matrix matrix{};
	setMatrixSize(matrix);
	system("cls");
	fillEmptySlots(matrix);
	drawGraph_With_Border(matrix);
	system("pause");
}

void part_6()
{
	Matrix matrix{};
	Matrix tempMatrix{};
	setMatrixSize(matrix);
	system("cls");
	tempMatrix.x = matrix.x;
	tempMatrix.y = matrix.y;
	fillEmptySlots(matrix);
	drawGraph_No_Border(matrix);
	system("pause");
	findElimination(matrix, tempMatrix);
	graph_setColorForElimination(matrix, tempMatrix);
	system("pause");
}

void part_7()
{
	Matrix matrix{};
	Matrix tempMatrix{};
	setMatrixSize(matrix);
	system("cls");
	tempMatrix.x = matrix.x;
	tempMatrix.y = matrix.y;
	fillEmptySlots(matrix);
	drawGraph_With_Border(matrix);
	system("pause");
	findElimination(matrix, tempMatrix);
	eliminateElements(matrix, tempMatrix);
	graph_eliminateElements(matrix, tempMatrix);
	system("pause");
	graph_markPotentialEliminations(matrix, tempMatrix);
	system("pause");
}

void part_8()
{
	Matrix matrix{};
	setMatrixSize(matrix);
	system("cls");
	cct_enable_mouse();
	fillEmptySlots(matrix);
	drawGraph_With_Border(matrix);
	while (true)
	{
		if (showMouseLocation(0, matrix.x * 2 + 1) == MOUSE_RIGHT_BUTTON_CLICK)
			break;
	}
	system("pause");
}


void part_9()
{
	Matrix matrix{};
	Matrix tempMatrix{};
	setMatrixSize(matrix);
	tempMatrix.x = matrix.x;
	tempMatrix.y = matrix.y;
	cct_setcursor(CURSOR_INVISIBLE);
	cct_enable_mouse();
	fillEmptySlots(matrix);
	drawGraph_With_Border(matrix);
	while (true)
	{
		findElimination(matrix, tempMatrix);
		if (graph_eliminateElements(matrix, tempMatrix) == 0)
			break;
		graph_fallTheBall(matrix);
		graph_fillEmptySlots(matrix);
	}
	int t = 1;
	while (true)
	{
		t = graph_markPotentialEliminations(matrix, tempMatrix);
		if (t == 0) break;
		if (useMouseSwapElement(matrix) == MOUSE_LEFT_BUTTON_CLICK)
		{
			while (true)
			{
				findElimination(matrix, tempMatrix);
				int num = graph_eliminateElements(matrix, tempMatrix);
				graph_fallTheBall(matrix);
				graph_fillEmptySlots(matrix);
				if (num == 0) break;
			}
		}
	}
	cct_gotoxy(0, matrix.x * 2 + 1);
	system("pause");
}
/*
1.内部数组，生成初始状态，寻找是否有初始可消除项
2.内部数组，消除初始可消除项后非0项下落并用0填充
3.内部数组，消除初始可消除项后查找消除提示
4.n*n的框架（无分隔线），显示初始状态
5.n*n的框架（有分隔线），显示初始状态
6.n*n的框架（有分隔线），显示初始状态及初始可消除项
7.n*的框架（有分隔线），消除初始可消除项后显示消除提示
8.cm图形界面完整版（有分隔线，鼠标移动时显示坐标，右键退出）
9.cmd图形界面完整版
0.退出

 */