#include "90-02-b1-magic_ball.h"
using namespace std;

int main()
{
	mainMenu();
	Matrix matrix{};
	matrix.x = 8;
	matrix.y = 8;
	drawGraph_With_Border(matrix);
	int x = 8;
	int y = 1;
	cct_showstr(x * 4 - 2, y * 2 - 1, "★", COLOR_CYAN, COLOR_BLACK);
	return 0;
}