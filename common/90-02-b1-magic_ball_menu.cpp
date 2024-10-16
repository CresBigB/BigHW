#include "../90-02-b1/90-02-b1-magic_ball.h"

void mainMenu()
{
	while (true)
	{
		cct_setconsoleborder(100, 30, 20, 8);
		cct_setfontsize("新宋体", 26, 13);
		system("cls");
		cout << "1.命令行方式找出可消除项并标识\n";
		cout << "2.命令行完成一次消除（分步骤显示）\n";
		cout << "3.命令行完成一关（分步骤显示）\n";
		cout << "4.伪图形界面下用鼠标选择一个色块（无分隔线）\n";
		cout << "5.伪图形界面下用鼠标选择一个色块（有分隔线\n";
		cout << "6.伪图形界面下完成一次合成（分步骤）\n";
		cout << "7.伪图形界面完整版\n";
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
			//case 8:part_8(); break;
			//case 9:part_9(); break;
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
		cout << "请输入行数（8-10）：";
		cin >> matrix.x;
		cout << "请输入列数（8-10）：";
		cin >> matrix.y;
		if (matrix.x >= 8 && matrix.y >= 8 && matrix.x <= 10 && matrix.y <= 10)
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
	showMatrix(matrix);
	cout << "请选择行和列：";
	int w, h;
	while (true)
	{
		cin >> w >> h;
		if (w > matrix.x || w <= 0 || h > matrix.y || h <= 0)
		{
			cout << "错误的选择\n";
			return;
		}
		else break;
	}
	findElimination(w - 1, h - 1, matrix, tempMatrix);
	//findElimination(matrix, tempMatrix);
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
	showMatrix(matrix);

	cout << "请选择行和列：";
	int w, h;
	while (true)
	{
		cin >> w >> h;
		if (w > matrix.x || w <= 0 || h > matrix.y || h <= 0)
		{
			cout << "错误的选择\n";
			return;
		}
		else break;
	}

	findElimination(w - 1, h - 1, matrix, tempMatrix);
	setColorForElimination(matrix, tempMatrix);
	char ch;
	cout << "请输入选项('N'放弃，'Y'执行消除,'Q'退出):";
	cin >> ch;
	int cnt = 0;
	if (ch == 'y' || ch == 'Y')
	{
		cnt += eliminateElements(matrix, tempMatrix);
		showMatrix(matrix);
		system("pause");
		fallTheBall(matrix);
		showMatrix(matrix);
		cout << "得分:" << cnt * cnt * 5;
	}
	system("pause");
	//fillEmptySlots(matrix);
	//showMatrix(matrix);
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
	int cnt = 0;

	for (int k = 1; k <= 2; k++)
		for (int i = 0; i < matrix.y; i++)
			matrix.mat[i][k] = 1;

	while (!isGameOver(matrix))
	{
		setEmpty(tempMatrix);
		showMatrix(matrix);
		cout << "请选择行和列：";
		int w, h;
		while (true)
		{
			cin >> w >> h;
			if (w > matrix.x || w <= 0 || h > matrix.y || h <= 0)
			{
				cout << "错误的选择\n";
				return;
			}
			else break;
		}

		findElimination(w - 1, h - 1, matrix, tempMatrix);
		setColorForElimination(matrix, tempMatrix);
		char ch;
		cout << "请输入选项('N'放弃，'Y'执行消除,'Q'退出):";
		cin >> ch;
		if (ch == 'y' || ch == 'Y')
		{
			cnt += eliminateElements(matrix, tempMatrix);
			showMatrix(matrix);
			system("pause");
			fallTheBall(matrix);
			showMatrix(matrix);

			while (true)
			{
				int idx = getEmptyColumnIdx(matrix);
				if (idx != -1)
				{
					for (int i = idx + 1; i < matrix.y; ++i)
						moveColumn(i, matrix);
					matrix.y--;
					matrix.emptyColumn++;
				}
				else break;
			}
		}
		system("pause");
	}
	cout << "得分:" << cnt * cnt * 5;
}
void part_4()
{
	Matrix matrix{};
	Matrix tempMatrix{};
	setMatrixSize(matrix);
	cct_enable_mouse();
	tempMatrix.x = matrix.x;
	tempMatrix.y = matrix.y;
	fillEmptySlots(matrix);
	drawGraph_No_Border(matrix);
	auto act = getMouseAction();
	while (isGameOver(matrix))
	{
		auto cnt = useMouseSelectElement(matrix, tempMatrix, act);
		if (act.mouse_action == MOUSE_LEFT_BUTTON_CLICK)
		{
			act = getMouseAction();
			if (act.mouse_action == MOUSE_LEFT_BUTTON_CLICK)
			{
				graph_eliminateElements(matrix, tempMatrix);
				graph_fallTheBall(matrix);
				break;
			}
			setEmpty(tempMatrix);
		}
	}
	cct_gotoxy(0, matrix.x * 2 + 1);
	system("pause");
}
void part_5()
{
	Matrix matrix{};
	Matrix tempMatrix{};
	setMatrixSize(matrix);
	cct_enable_mouse();
	tempMatrix.x = matrix.x;
	tempMatrix.y = matrix.y;
	fillEmptySlots(matrix);
	drawGraph_With_Border(matrix);
	auto act = getMouseAction();
	while (isGameOver(matrix))
	{
		auto cnt = useMouseSelectElement(matrix, tempMatrix, act);
		if (act.mouse_action == MOUSE_LEFT_BUTTON_CLICK)
		{
			act = getMouseAction();
			if (act.mouse_action == MOUSE_LEFT_BUTTON_CLICK)
			{
				graph_eliminateElements(matrix, tempMatrix);
				graph_fallTheBall(matrix);
				break;
			}
			setEmpty(tempMatrix);
		}
	}
	cct_gotoxy(0, matrix.x * 2 + 1);
	system("pause");
}

void part_6()
{
	Matrix matrix{};
	Matrix tempMatrix{};
	setMatrixSize(matrix);
	cct_enable_mouse();
	tempMatrix.x = matrix.x;
	tempMatrix.y = matrix.y;
	fillEmptySlots(matrix);
	drawGraph_With_Border(matrix);
	auto act = getMouseAction();
	while (isGameOver(matrix))
	{
		auto cnt = useMouseSelectElement(matrix, tempMatrix, act);
		if (act.mouse_action == MOUSE_LEFT_BUTTON_CLICK)
		{
			auto act = getMouseAction();
			if (act.mouse_action == MOUSE_LEFT_BUTTON_CLICK)
			{
				graph_eliminateElements(matrix, tempMatrix);
				graph_fallTheBall(matrix);
				break;
			}
			setEmpty(tempMatrix);
		}
	}
	cct_gotoxy(0, matrix.x * 2 + 1);
	system("pause");
}

void part_7()
{
	Matrix matrix{};
	Matrix tempMatrix{};
	setMatrixSize(matrix);
	cct_enable_mouse();
	tempMatrix.x = matrix.x;
	tempMatrix.y = matrix.y;
	fillEmptySlots(matrix);
	for (int k = 1; k <= 2; k++)
		for (int i = 0; i < matrix.y; i++)
			matrix.mat[i][k] = 1;

	drawGraph_With_Border(matrix);
	auto act = getMouseAction();
	act.mouse_x = 1;
	act.mouse_y = 1;

	while (!isGameOver(matrix))
	{
		setEmpty(tempMatrix);

		while (true)
		{
			act = getMouseAction();
			keyHit(act);
			showMouseLocation(matrix, act.mouse_x, act.mouse_y);
			if (act.mouse_action == MOUSE_LEFT_BUTTON_CLICK)
			{
				if (act.mouse_x > 0 && act.mouse_y > 0 && act.mouse_x <= matrix.x && act.mouse_y <= matrix.y)
					findElimination(act.mouse_x - 1, act.mouse_y - 1, matrix, tempMatrix);
				graph_setColorForElimination(matrix, tempMatrix);
				break;
			}
		}

		while (true)
		{
			auto act2 = getMouseAction();
			keyHit(act2);
			if (act2.mouse_action == MOUSE_LEFT_BUTTON_CLICK)
			{
				graph_eliminateElements(matrix, tempMatrix);
				graph_fallTheBall(matrix);
				bool t = false;
				while (true)
				{
					auto idx = getEmptyColumnIdx(matrix);
					if (idx != -1)
					{
						t = true;
						for (int i = idx + 1; i < matrix.y; ++i)
							graph_moveColumn(i, matrix);
						matrix.y--;
						matrix.emptyColumn++;
					}
					else break;
				}
				break;
			}
			else if (act2.mouse_x != act.mouse_x || act.mouse_y != act2.mouse_y)
			{
				setEmpty(tempMatrix);
				graph_setColorForElimination(matrix, tempMatrix);
				break;
			}
		}

	}
	cct_gotoxy(0, matrix.x * 2 + 1);
	cout << "游戏结束，得分为：" << tempMatrix.emptyColumn * tempMatrix.emptyColumn * 5 << "\n"
		<< "剩余星星数量：" << countItem(matrix) << "\n";
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