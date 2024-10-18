/* 学号 姓名 班级 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"

#include "../90-01-b1/90-01-b1-hanoi.h"
using namespace std;

/* --------------------------------------------------
		此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */


   /* --------------------------------------------------
		   此处可以给出需要的内部辅助工具函数
		   1、函数名不限，建议为 gmw_inner_*
		   2、个数不限
		   3、必须是static函数，确保只在本源文件中使用
	  -------------------------------------------------- */


	  /* -----------------------------------------------
			  实现下面给出的函数（函数声明不准动）
		 ----------------------------------------------- */

static void updateCGI(CONSOLE_GRAPHICS_INFO* const pCGI)
{
	pCGI->start_y = pCGI->draw_frame_with_col_no + pCGI->SLI.is_top_status_line + pCGI->extern_up_lines;
	pCGI->start_x = pCGI->draw_frame_with_row_no + pCGI->extern_left_cols;

	pCGI->CFI.bwidth = pCGI->CFI.block_width * pCGI->col_num + 4;
	pCGI->CFI.bhigh = pCGI->CFI.block_high * pCGI->row_num + 2;

	if (pCGI->CFI.separator)
	{
		pCGI->CFI.bwidth += 2 * pCGI->col_num;
		pCGI->CFI.bhigh += pCGI->row_num;
		pCGI->CFI.bwidth -= 3;
		pCGI->CFI.bhigh -= 1;
	}

	pCGI->SLI.lower_start_x = pCGI->extern_left_cols;
	pCGI->SLI.lower_start_y = pCGI->CFI.bhigh + pCGI->start_y;
	pCGI->SLI.top_start_x = pCGI->extern_left_cols;
	pCGI->SLI.top_start_y = pCGI->extern_up_lines;

	pCGI->lines = 1 + pCGI->CFI.bhigh + pCGI->SLI.is_lower_status_line + pCGI->SLI.is_top_status_line + pCGI->extern_down_lines + pCGI->extern_up_lines + pCGI->draw_frame_with_row_no;
	pCGI->cols = 2 + pCGI->CFI.bwidth + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_col_no;

}

/***************************************************************************
  函数名称：
  功    能：设置游戏主框架的行列数
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int row						：行数(错误则为0，不设上限，人为保证正确性)
			const int col						：列数(错误则为0，不设上限，人为保证正确性)
  返 回 值：
  说    明：1、指消除类游戏的矩形区域的行列值
			2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->row_num = row;
	pCGI->col_num = col;

	updateCGI(pCGI);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bg_color					：前景色（缺省COLOR_BLACK）
		   const int fg_color					：背景色（缺省COLOR_WHITE）
		   const bool cascade					：是否级联（缺省为true-级联）
  返 回 值：
  说    明：1、cascade = true时
				同步修改游戏主区域的颜色
				同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
			2、不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15
					前景色背景色的值一致导致无法看到内容
					前景色正好是状态栏醒目前景色，导致无法看到醒目提示
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	if (cascade)
	{
		pCGI->SLI.lower_normal_bgcolor = bgcolor;
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;
		pCGI->SLI.lower_normal_fgcolor = fgcolor;
		pCGI->SLI.top_normal_bgcolor = bgcolor;
		pCGI->SLI.top_catchy_bgcolor = bgcolor;
		pCGI->SLI.top_normal_fgcolor = fgcolor;

		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
		   const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
		   const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
			2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO* const pCGI, const char* fontname, const int fs_high, const int fs_width)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (!strcmp(fontname, "Terminal") && !strcmp(fontname, "新宋体"))
		return -1;

	// 设置字体大小，默认高度为 16，宽度为 8
	pCGI->CFT.font_size_width = (fs_width > 0) ? fs_width : 8;
	pCGI->CFT.font_size_high = 2 * pCGI->CFT.font_size_width;  // 强制设置 height 为 width 的两倍

	//if (fs_high <= 0 || fs_width <= 0)
	//{
	//	pCGI->CFT.font_size_high = 16;
	//	pCGI->CFT.font_size_width = 8;
	//	return -1;
	//}
	//if (fs_width * 2 < fs_high)
	//{
	//	pCGI->CFT.font_size_width = fs_high * 2;
	//	pCGI->CFT.font_size_high = fs_high;
	//}
	//else if (fs_width * 2 > fs_high)
	//{
	//	pCGI->CFT.font_size_width = fs_width;
	//	pCGI->CFT.font_size_high = fs_width / 2;
	//}
	//else
	//{
	//	pCGI->CFT.font_size_width = fs_width;
	//	pCGI->CFT.font_size_high = fs_high;
	//}

	strcpy(pCGI->CFT.font_type, fontname);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type						：延时的类型（目前为3种）
		   const int delay_ms					：以ms为单位的延时
			   画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const int delay_ms)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	switch (type)
	{
	case DELAY_OF_DRAW_FRAME:pCGI->delay_of_draw_frame = delay_ms; break;
	case DELAY_OF_DRAW_BLOCK:pCGI->delay_of_draw_block = delay_ms; break;
	case DELAY_OF_BLOCK_MOVED:pCGI->delay_of_block_moved = delay_ms; break;
	default:
		return -1;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines					：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols					：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO* const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	if (up_lines < 0 || down_lines < 0 || left_cols < 0 || right_cols < 0)
		return -1;

	if (up_lines >= 0)
	{
		pCGI->extern_up_lines = up_lines;
		//pCGI->SLI.top_start_y = up_lines;
	}
	if (down_lines >= 0)
	{
		pCGI->extern_down_lines = down_lines;
	}
	if (left_cols >= 0)
	{
		pCGI->extern_left_cols = left_cols;
		//pCGI->SLI.top_start_x = left_cols;
		//pCGI->SLI.lower_start_x = left_cols;
	}
	if (right_cols >= 0)
		pCGI->extern_right_cols = right_cols;

	updateCGI(pCGI);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const int type)
{
	// 防止在未调用 gmw_init 前调用其它函数
	if (pCGI->inited != CGI_INITED)
		return -1;

	// 根据 type 设置不同的线型
	switch (type)
	{
	case 1: // 双线框架
		strncpy(pCGI->CFI.top_left, "╔", CFI_LEN);
		strncpy(pCGI->CFI.lower_left, "╚", CFI_LEN);
		strncpy(pCGI->CFI.top_right, "╗", CFI_LEN);
		strncpy(pCGI->CFI.lower_right, "╝", CFI_LEN);
		strncpy(pCGI->CFI.h_normal, "═", CFI_LEN);
		strncpy(pCGI->CFI.v_normal, "║", CFI_LEN);
		strncpy(pCGI->CFI.h_top_separator, "╦", CFI_LEN);
		strncpy(pCGI->CFI.h_lower_separator, "╩", CFI_LEN);
		strncpy(pCGI->CFI.v_left_separator, "╠", CFI_LEN);
		strncpy(pCGI->CFI.v_right_separator, "╣", CFI_LEN);
		strncpy(pCGI->CFI.mid_separator, "╬", CFI_LEN);
		break;

	case 2: // 单线框架
		strncpy(pCGI->CFI.top_left, "┏", CFI_LEN);
		strncpy(pCGI->CFI.lower_left, "┗", CFI_LEN);
		strncpy(pCGI->CFI.top_right, "┓", CFI_LEN);
		strncpy(pCGI->CFI.lower_right, "┛", CFI_LEN);
		strncpy(pCGI->CFI.h_normal, "━", CFI_LEN);
		strncpy(pCGI->CFI.v_normal, "┃", CFI_LEN);
		strncpy(pCGI->CFI.h_top_separator, "┳", CFI_LEN);
		strncpy(pCGI->CFI.h_lower_separator, "┻", CFI_LEN);
		strncpy(pCGI->CFI.v_left_separator, "┣", CFI_LEN);
		strncpy(pCGI->CFI.v_right_separator, "┫", CFI_LEN);
		strncpy(pCGI->CFI.mid_separator, "╋", CFI_LEN);
		break;

	case 3: // 横双竖单
		strncpy(pCGI->CFI.top_left, "╒", CFI_LEN);
		strncpy(pCGI->CFI.lower_left, "╘", CFI_LEN);
		strncpy(pCGI->CFI.top_right, "╕", CFI_LEN);
		strncpy(pCGI->CFI.lower_right, "╛", CFI_LEN);
		strncpy(pCGI->CFI.h_normal, "═", CFI_LEN);
		strncpy(pCGI->CFI.v_normal, "┃", CFI_LEN); // 使用单竖线
		strncpy(pCGI->CFI.h_top_separator, "╤", CFI_LEN);
		strncpy(pCGI->CFI.h_lower_separator, "╧", CFI_LEN);
		strncpy(pCGI->CFI.v_left_separator, "╞", CFI_LEN);
		strncpy(pCGI->CFI.v_right_separator, "╡", CFI_LEN);
		strncpy(pCGI->CFI.mid_separator, "╪", CFI_LEN);
		break;

	case 4: // 横单竖双
		strncpy(pCGI->CFI.top_left, "╓", CFI_LEN);
		strncpy(pCGI->CFI.lower_left, "╙", CFI_LEN);
		strncpy(pCGI->CFI.top_right, "╖", CFI_LEN);
		strncpy(pCGI->CFI.lower_right, "╜", CFI_LEN);
		strncpy(pCGI->CFI.h_normal, "─", CFI_LEN);
		strncpy(pCGI->CFI.v_normal, "║", CFI_LEN); // 使用双竖线
		strncpy(pCGI->CFI.h_top_separator, "╥", CFI_LEN);
		strncpy(pCGI->CFI.h_lower_separator, "╨", CFI_LEN);
		strncpy(pCGI->CFI.v_left_separator, "╟", CFI_LEN);
		strncpy(pCGI->CFI.v_right_separator, "╢", CFI_LEN);
		strncpy(pCGI->CFI.mid_separator, "╫", CFI_LEN);
		break;

	default:
		return -1; // 不支持的类型
	}

	return 0; // 返回成功
}


/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/

static void process_char_input(const char* input, char* output)
{
	if (input == NULL)
	{
		// 用两个空格替代
		output[0] = ' ';
		output[1] = ' ';
	}
	else
	{
		// 计算输入字符的字节数
		size_t len = strlen(input);
		if (len >= 2)
		{
			// 超过2字节则只取前2字节
			output[0] = input[0];
			output[1] = input[1];
		}
		else if (len == 1)
		{
			// 如果给1字节，则补一个空格
			output[0] = input[0];
			output[1] = ' ';
		}
		else
		{
			// 输入为空或其他情况
			output[0] = ' ';
			output[1] = ' ';
		}
	}
};

int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left,
	const char* top_right, const char* lower_right, const char* h_normal,
	const char* v_normal, const char* h_top_separator, const char* h_lower_separator,
	const char* v_left_separator, const char* v_right_separator, const char* mid_separator)
{
	// 防止在未调用 gmw_init 前调用其它函数
	if (pCGI->inited != CGI_INITED)
		return -1;

	// 处理各个输入并填充到结构中
	process_char_input(top_left, pCGI->CFI.top_left);
	process_char_input(lower_left, pCGI->CFI.lower_left);
	process_char_input(top_right, pCGI->CFI.top_right);
	process_char_input(lower_right, pCGI->CFI.lower_right);
	process_char_input(h_normal, pCGI->CFI.h_normal);
	process_char_input(v_normal, pCGI->CFI.v_normal);
	process_char_input(h_top_separator, pCGI->CFI.h_top_separator);
	process_char_input(h_lower_separator, pCGI->CFI.h_lower_separator);
	process_char_input(v_left_separator, pCGI->CFI.v_left_separator);
	process_char_input(v_right_separator, pCGI->CFI.v_right_separator);
	process_char_input(mid_separator, pCGI->CFI.mid_separator);

	return 0; // 返回成功
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要┏1）
			const int block_high						：高度（错误及缺省1）
			const bool separator						：是否需要分隔线（缺省为true，需要分隔线）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO* const pCGI, const int block_width, const int block_high, const bool separator)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->CFI.bwidth = block_width % 2 ? block_width + 1 : block_width * pCGI->col_num + 1;
	pCGI->CFI.bhigh = block_high * pCGI->row_num + 1;

	if (separator)
	{
		pCGI->CFI.bwidth += 2 * pCGI->col_num;
		pCGI->CFI.bhigh += pCGI->row_num;
	}
	//if (pCGI->CFI.separator == true && !separator)
	//{
	//	pCGI->CFI.bwidth -= (block_width % 2 ? block_width + 1 : block_width) * (pCGI->col_num) + 1;
	//	pCGI->CFI.bhigh -= pCGI->row_num * block_high + 1;
	//}
	//pCGI->CFI.bwidth += ((block_width % 2 ? block_width + 1 : block_width) - pCGI->CFI.block_width) * pCGI->col_num;
	//pCGI->CFI.bhigh += (block_high - pCGI->CFI.block_high) * pCGI->row_num;
	pCGI->CFI.bwidth = pCGI->CFI.bwidth >= 0 ? pCGI->CFI.bwidth : 0;
	pCGI->CFI.bhigh = pCGI->CFI.bhigh >= 0 ? pCGI->CFI.bhigh : 0;

	pCGI->CFI.separator = separator;
	pCGI->CFI.block_width = block_width % 2 ? block_width + 1 : block_width;
	pCGI->CFI.block_high = block_high;

	updateCGI(pCGI);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int bg_color					：背景色（缺省 -1表示用窗口背景色）
			const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int bgcolor, const int fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	pCGI->CFI.bgcolor = bgcolor;
	pCGI->CFI.fgcolor = fgcolor;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const int type)
{
	if (pCGI->inited != CGI_INITED)
		return -1;

	switch (type)
	{
	case 1: // 全双线
		strcpy(pCGI->CBI.top_left, "╔");
		strcpy(pCGI->CBI.top_right, "╗");
		strcpy(pCGI->CBI.lower_left, "╚");
		strcpy(pCGI->CBI.lower_right, "╝");
		strcpy(pCGI->CBI.h_normal, "══");
		strcpy(pCGI->CBI.v_normal, "║");
		break;

	case 2: // 全单线
		strcpy(pCGI->CBI.top_left, "┌");
		strcpy(pCGI->CBI.top_right, "┐");
		strcpy(pCGI->CBI.lower_left, "└");
		strcpy(pCGI->CBI.lower_right, "┘");
		strcpy(pCGI->CBI.h_normal, "──");
		strcpy(pCGI->CBI.v_normal, "│");
		break;

	case 3: // 横双竖单
		strcpy(pCGI->CBI.top_left, "╔");
		strcpy(pCGI->CBI.top_right, "╗");
		strcpy(pCGI->CBI.lower_left, "╚");
		strcpy(pCGI->CBI.lower_right, "╝");
		strcpy(pCGI->CBI.h_normal, "══");
		strcpy(pCGI->CBI.v_normal, "│");
		break;

	case 4: // 横单竖双
		strcpy(pCGI->CBI.top_left, "┌");
		strcpy(pCGI->CBI.top_right, "┐");
		strcpy(pCGI->CBI.lower_left, "└");
		strcpy(pCGI->CBI.lower_right, "┘");
		strcpy(pCGI->CBI.h_normal, "──");
		strcpy(pCGI->CBI.v_normal, "║");
		break;

	default:
		return -1; // 返回错误代码
	}

	return 0; // 成功
}
/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left, const char* top_right,
	const char* lower_right, const char* h_normal, const char* v_normal)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	// 处理各个输入并填充到结构中
	process_char_input(top_left, pCGI->CBI.top_left);
	process_char_input(lower_left, pCGI->CBI.lower_left);
	process_char_input(top_right, pCGI->CBI.top_right);
	process_char_input(lower_right, pCGI->CBI.lower_right);
	process_char_input(h_normal, pCGI->CBI.h_normal);
	process_char_input(v_normal, pCGI->CBI.v_normal);
	return 0; // 成功
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off					：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	// 根据 type 设置不同的线型
	if (on_off)
	{
		strncpy(pCGI->CBI.top_left, "╔", CFI_LEN);
		strncpy(pCGI->CBI.lower_left, "╚", CFI_LEN);
		strncpy(pCGI->CBI.top_right, "╗", CFI_LEN);
		strncpy(pCGI->CBI.lower_right, "╝", CFI_LEN);
		strncpy(pCGI->CBI.h_normal, "═", CFI_LEN);
		strncpy(pCGI->CBI.v_normal, "║", CFI_LEN);
	}

	pCGI->CBI.block_border = on_off;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const bool on_off					：true - 需要 flase - 不需要（缺省true）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
			   1.1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
			   1.2、下状态栏只能一行，在主区域最下方框线的下面
			   1.3、状态栏的宽度为主区域宽度，如果信息过长则截断
		   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == TOP_STATUS_LINE)
	{
		pCGI->top_status_line = on_off;
		pCGI->SLI.is_top_status_line = on_off;
	}
	if (type == LOWER_STATUS_LINE)
	{
		pCGI->lower_status_line = on_off;
		pCGI->SLI.is_lower_status_line = on_off;
	}

	updateCGI(pCGI);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
			const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
			const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
			const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (type == TOP_STATUS_LINE)
	{
		pCGI->SLI.top_normal_bgcolor = normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.top_catchy_bgcolor = catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.top_normal_fgcolor = normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.top_catchy_fgcolor = catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor;
	}

	if (type == LOWER_STATUS_LINE)
	{
		pCGI->SLI.lower_normal_bgcolor = normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.lower_catchy_bgcolor = catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.lower_normal_fgcolor = normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.lower_catchy_fgcolor = catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor;
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
			2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (on_off && !pCGI->draw_frame_with_row_no)
	{
		//pCGI->extern_up_lines += 2;
		pCGI->start_x = 2;
	}

	if (!on_off && pCGI->draw_frame_with_row_no)
	{
		//pCGI->extern_down_lines -= 2;
		pCGI->start_y = 0;
	}

	pCGI->draw_frame_with_row_no = on_off;

	updateCGI(pCGI);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
			2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	if (on_off && !pCGI->draw_frame_with_row_no)
	{
		//pCGI->extern_down_lines += 1;
		pCGI->start_y += 1;
	}

	if (!on_off && pCGI->draw_frame_with_row_no)
	{
		//pCGI->extern_down_lines -= 1;
		pCGI->start_y -= 1;
	}

	pCGI->draw_frame_with_col_no = on_off;

	updateCGI(pCGI);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
			2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	memset(pCGI, 0, sizeof(*pCGI));

	/* 首先置标记 */
	pCGI->inited = CGI_INITED;

	gmw_set_rowcol(pCGI, row, col);
	gmw_set_ext_rowcol(pCGI);
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE, true);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE, true);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE);
	gmw_set_frame_default_linetype(pCGI, 1);
	gmw_set_font(pCGI, "新宋体", 16, 8);
	gmw_set_color(pCGI, bgcolor, fgcolor);
	gmw_set_block_border_switch(pCGI);
	gmw_set_frame_style(pCGI);
	gmw_set_ext_rowcol(pCGI, 0, 3, 0, 0);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE, pCGI->area_bgcolor, pCGI->area_fgcolor,
		pCGI->area_bgcolor, COLOR_HYELLOW);
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE, pCGI->area_bgcolor, pCGI->area_fgcolor,
		pCGI->area_bgcolor, COLOR_HYELLOW);
	//updateCGI(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO* const pCGI)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	//cct_setconsoleborder(pCGI->cols, pCGI->lines, -1, -1);

	// 设置字体、边框、颜色，并清屏
	cct_setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	if (strcmp(pCGI->CFT.font_type, "") == 0)
		cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, 0);
	else
		cct_setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);

	cct_setconsoleborder(pCGI->cols, pCGI->lines, -1, -1);
	//cct_setconsoleborder(50, 20, -1, -1);
	cct_cls();

	if (pCGI->draw_frame_with_col_no)
	{
		int x = pCGI->start_x + pCGI->CFI.block_width / 2 + 2;
		int y = pCGI->start_y - 1;

		for (int i = 0; i < pCGI->CFI.bwidth; ++i)
			cct_showstr(pCGI->start_x + i, pCGI->start_y - 1, " ", pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);

		if (!pCGI->CFI.separator)
			x -= 2;
		char text[3] = { 0 };
		for (int i = 0; i < pCGI->col_num; ++i)
		{
			if (i <= 99)
				snprintf(text, 3, "%d", i);
			else
				strcpy(text, "**");
			cct_showstr(x, y, text, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			if (pCGI->CFI.separator)
				x += 2;
			x += pCGI->CFI.block_width;
		}
	}

	if (pCGI->draw_frame_with_row_no)
	{
		int x = pCGI->start_x - 1;
		int y = pCGI->start_y + pCGI->CFI.block_high / 2 + 1;

		for (int i = 0; i < pCGI->CFI.bhigh; ++i)
			cct_showstr(pCGI->start_x - 1, pCGI->start_y + i, " ", pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);


		if (!pCGI->CFI.separator)
			y -= 1;
		char text[3] = { 0 };
		for (int i = 0; i < pCGI->row_num; ++i)
		{
			if (i <= 25)
				snprintf(text, 3, "%c", 'A' + i);
			else
				strcpy(text, "*");
			cct_showstr(x, y, text, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			if (pCGI->CFI.separator)
				y += 1;
			y += pCGI->CFI.block_high;
		}
	}

	for (int i = pCGI->start_y; i < pCGI->CFI.bhigh + pCGI->start_y; i += 1)
	{
		for (int j = pCGI->start_x; j <= pCGI->start_x + pCGI->CFI.bwidth; j += 1)
			cct_showstr(j, i, " ", pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
	}

	for (int i = pCGI->start_y; i < pCGI->CFI.bhigh + pCGI->start_y; i += 1)
	{
		for (int j = pCGI->start_x; j < pCGI->start_x + pCGI->CFI.bwidth; j += 1)
		{
			Sleep(pCGI->delay_of_draw_frame);
			//左上
			if (i == pCGI->start_y && j == pCGI->start_x)
				cct_showstr(j, i, pCGI->CFI.top_left, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			//右上
			if (j == pCGI->start_x + pCGI->CFI.bwidth - 1 && i == pCGI->start_y)
				cct_showstr(j, i, pCGI->CFI.top_right, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			//左下
			if (i == pCGI->CFI.bhigh + pCGI->start_y - 1 && j == pCGI->start_x)
				cct_showstr(j, i, pCGI->CFI.lower_left, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			//右下
			if (i == pCGI->CFI.bhigh + pCGI->start_y - 1 && j == pCGI->start_x + pCGI->CFI.bwidth - 1)
				cct_showstr(j, i, pCGI->CFI.lower_right, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			//最上下横线
			if ((i == pCGI->start_y || i == pCGI->CFI.bhigh + pCGI->start_y - 1) && j != pCGI->start_x && j != pCGI->start_x + pCGI->CFI.bwidth - 1)
				cct_showstr(j, i, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			//最左右竖线
			if ((j == pCGI->start_x || j == pCGI->start_x + pCGI->CFI.bwidth - 1) && i != pCGI->start_y && i != pCGI->CFI.bhigh + pCGI->start_y - 1)
				cct_showstr(j, i, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);

			if (pCGI->CFI.separator)
			{
				if (j != pCGI->start_x && j != pCGI->start_x + pCGI->CFI.bwidth - 1)
				{
					//最上方分割线
					if (i == pCGI->start_y && j != pCGI->start_x && (j - pCGI->start_x) != pCGI->start_x + pCGI->CFI.bwidth - 1
						&& (j - pCGI->start_x) % (pCGI->CFI.block_width + 2) == 0)
						cct_showstr(j, i, pCGI->CFI.h_top_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					//最下方分割线
					if (i == pCGI->CFI.bhigh + pCGI->start_y - 1 && (j - pCGI->start_x) != pCGI->start_x
						&& j != pCGI->start_x + pCGI->CFI.bwidth - 1 && (j - pCGI->start_x) % (pCGI->CFI.block_width + 2) == 0)
						cct_showstr(j, i, pCGI->CFI.h_lower_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				}
				//最左方分割线
				if (j == pCGI->start_x && i != pCGI->start_y && i != pCGI->CFI.bhigh + pCGI->start_y - 1
					&& (i - pCGI->start_y) % (pCGI->CFI.block_high + 1) == 0)
				{
					cct_showstr(j, i, pCGI->CFI.v_left_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					//十字线和横线
					bool isNormal = true;
					for (int k = pCGI->start_x + 1; k < pCGI->start_x + pCGI->CFI.bwidth - 1; k += 1)
					{
						Sleep(pCGI->delay_of_draw_frame);

						if (!isNormal)
							cct_showstr(k, i, pCGI->CFI.mid_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
						else
						{
							for (int l = 0; l < pCGI->CFI.block_width; ++l, ++k)
							{
								cct_showstr(k, i, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
								Sleep(pCGI->delay_of_draw_frame);

							}
						}
						isNormal = !isNormal;
					}
				}
				//最右方分割线
				if (j == pCGI->start_x + pCGI->CFI.bwidth - 1 && i != pCGI->start_y && i != pCGI->CFI.bhigh + pCGI->start_y - 1
					&& (i - pCGI->start_y) % (pCGI->CFI.block_high + 1) == 0)
					cct_showstr(j, i, pCGI->CFI.v_right_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				//分割线竖线
				if ((i - pCGI->start_y) % (pCGI->CFI.block_high + 1) && (j - pCGI->start_x) % (pCGI->CFI.block_width + 2) == 0)
					cct_showstr(j, i, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}
		}
	}

	return 0; //此句可根据需要修改
}

// 判断字符是否为UTF-8多字节字符的后续字节
static int is_utf8_continuation_byte(char c)
{
	return (c & 0xC0) == 0x80;  // UTF-8续字节以'10'开头
}

// 截取指定字节长度的字符串，避免截断中文字符
static void safe_strncpy(char* dest, const char* src, size_t n)
{
	size_t byte_count = 0;  // 已处理的字节数
	size_t i = 0;           // 当前源字符串的索引

	while (src[i] && byte_count < n)
	{
		// 如果下一个字节属于一个完整的UTF-8字符的开头，就拷贝
		if (!is_utf8_continuation_byte(src[i]))
		{
			// 如果这个字符会导致超出n个字节，则停止
			if (byte_count + (strlen(src + i) < n - byte_count ? strlen(src + i) : n - byte_count) > n)
				break;
		}

		// 逐字节拷贝
		dest[byte_count++] = src[i++];
	}

	dest[byte_count] = '\0';  // 加上终止符
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type							：指定是上/下状态栏
		   const char *msg						：正常信息
		   const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
			2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const char* msg, const char* catchy_msg)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	char* message = new char[pCGI->cols + 1] {0};
	safe_strncpy(message, msg, pCGI->cols % 2 ? pCGI->cols - 1 : pCGI->cols);

	if (type == TOP_STATUS_LINE && pCGI->top_status_line)
	{
		int len = 0;
		if (catchy_msg)
		{
			cct_showstr(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y, catchy_msg,
				pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
			len = strlen(catchy_msg);
		}

		cct_showstr(pCGI->SLI.top_start_x + len, pCGI->SLI.top_start_y, message,
			pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor);
	}

	if (type == LOWER_STATUS_LINE && pCGI->lower_status_line)
	{
		int len = 0;
		if (catchy_msg)
		{
			cct_showstr(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y, catchy_msg,
				pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
			len = strlen(catchy_msg);
		}

		cct_showstr(pCGI->SLI.lower_start_x + len, pCGI->SLI.lower_start_y, message,
			pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no							：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no							：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
			2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
static void draw_block_xy(const CONSOLE_GRAPHICS_INFO* const pCGI, const int start_x,
	const int start_y, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi)
{
	if (bdi_value == BDI_VALUE_END) return;

	int idx = 0;
	while (bdi[idx].value != BDI_VALUE_END)
	{
		if (bdi[idx].value == bdi_value)
			break;
		++idx;
	}
	if (bdi[idx].value == BDI_VALUE_END) return;

	char* text = new char[30] {0};
	if (bdi[idx].content == nullptr)
		snprintf(text, 30, " %d", bdi[idx].value);
	else
		snprintf(text, 30, "%s", bdi[idx].content);

	auto bc = bdi[idx].bgcolor == -1 ? pCGI->area_bgcolor : bdi[idx].bgcolor;			//色块背景色
	auto fc = bdi[idx].fgcolor == -1 ? pCGI->area_fgcolor : bdi[idx].fgcolor;			//色块前景色

	if (pCGI->CFI.separator)
	{
		if (bdi_value == BDI_VALUE_BLANK)
		{
			for (int i = 0; i < pCGI->CFI.block_width; ++i)
			{
				for (int j = 0; j < pCGI->CFI.block_high; ++j)
				{
					cct_showstr(start_x + i, start_y + j, " ", pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					Sleep(pCGI->delay_of_draw_block);
				}
			}
		}
		else
		{
			for (int i = 0; i < pCGI->CFI.block_width; ++i)
			{
				for (int j = 0; j < pCGI->CFI.block_high; ++j)
				{
					cct_showstr(start_x + i, start_y + j, " ", bc, fc);
					Sleep(pCGI->delay_of_draw_block);
				}
			}
			cct_showstr(start_x + pCGI->CFI.block_width / 2 - 1, start_y + pCGI->CFI.block_high / 2,
				bdi_value != BDI_VALUE_BLANK ? text : "  ", bc, fc);
		}

	}
	else
	{
		if (bdi_value == BDI_VALUE_BLANK)
		{
			for (int i = 0; i < pCGI->CFI.block_width; ++i)
			{
				for (int j = 0; j < pCGI->CFI.block_high; ++j)
				{
					cct_showstr(start_x + i, start_y + j, " ", pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					Sleep(pCGI->delay_of_draw_block);
				}
			}
		}
		else
		{
			for (int i = 0; i < pCGI->CFI.block_width; ++i)
			{
				for (int j = 0; j < pCGI->CFI.block_high; ++j)
				{
					cct_showstr(start_x + i, start_y + j, " ", bc, fc);
					Sleep(pCGI->delay_of_draw_block);
				}
			}
			cct_showstr(start_x + pCGI->CFI.block_width / 2 - 1, start_y + pCGI->CFI.block_high / 2,
				bdi_value != BDI_VALUE_BLANK ? text : "  ", bc, fc);
		}
	}

	if (pCGI->CBI.block_border && bdi_value != BDI_VALUE_BLANK)
	{
		for (int i = 0; i < pCGI->CFI.block_width - 1; ++i)
		{
			for (int j = 0; j < pCGI->CFI.block_high; ++j)
			{
				//坐上
				if (i == 0 && j == 0)
					cct_showstr(start_x + i, start_y + j, pCGI->CBI.top_left, bc, fc);
				//坐下
				else if (i == 0 && j == pCGI->CFI.block_high - 1)
					cct_showstr(start_x + i, start_y + j, pCGI->CBI.lower_left, bc, fc);
				//右上
				else if (i == pCGI->CFI.block_width - 2 && j == 0)
					cct_showstr(start_x + i, start_y + j, pCGI->CBI.top_right, bc, fc);
				//右下
				else if (i == pCGI->CFI.block_width - 2 && j == pCGI->CFI.block_high - 1)
					cct_showstr(start_x + i, start_y + j, pCGI->CBI.lower_right, bc, fc);
				//上下横线
				else if ((i > 0 || i < pCGI->CFI.block_width - 2) && (j == 0 || j == pCGI->CFI.block_high - 1))
					cct_showstr(start_x + i, start_y + j, pCGI->CBI.h_normal, bc, fc);
				//左右竖线
				else if ((j > 0 || j < pCGI->CFI.block_high - 1) && (i == 0 || i == pCGI->CFI.block_width - 2))
					cct_showstr(start_x + i, start_y + j, pCGI->CBI.v_normal, bc, fc);
				Sleep(pCGI->delay_of_draw_block);
			}
		}
	}

	delete[] text;

}

int gmw_draw_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	/*找位置*/
	int block_start_x0 = pCGI->start_x + 2;
	int block_start_y0 = pCGI->start_y + 1;//有无分界线都是一样
	int block_start_x = block_start_x0 + col_no * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);//给定行列(row_no,col_no)位置的色块起始坐标
	int block_start_y = block_start_y0 + row_no * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);
	draw_block_xy(pCGI, block_start_x, block_start_y, bdi_value, bdi);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no							：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no							：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
		   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
		   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value,
	const int blank_bdi_value, const BLOCK_DISPLAY_INFO* const bdi, const int direction, const int distance)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	constexpr int dir[4][2] = { {-1,0} ,{1,0},{0,-1},{0,1} };

	int idx = 0;
	while (bdi[idx].value != BDI_VALUE_END)
	{
		if (bdi[idx].value == bdi_value)
			break;
		++idx;
	}
	if (bdi[idx].value == BDI_VALUE_END) return -1;

	int row = row_no;
	int col = col_no;
	auto bc = bdi[idx].bgcolor == -1 ? pCGI->area_bgcolor : bdi[idx].bgcolor;	//色块背景色
	auto fc = bdi[idx].fgcolor == -1 ? pCGI->area_fgcolor : bdi[idx].fgcolor;	//色块前景色
	int x = pCGI->start_x + col_no * (pCGI->CFI.block_width + 2) + 2;				//色块X坐标
	int y = pCGI->start_y + row_no * (pCGI->CFI.block_high + 1) + 1;				//色块Y坐标

	if (!pCGI->CFI.separator)
	{
		x = pCGI->start_x + col_no * pCGI->CFI.block_width + 2;
		y = pCGI->start_y + row_no * pCGI->CFI.block_high + 1;
	}

	int dis = 0;

	switch (direction)
	{
	case UP_TO_DOWN:
		dis = distance * pCGI->CFI.block_high + (distance)*pCGI->CFI.separator;
		for (int i = 0; i < dis; ++i)
		{
			draw_block_xy(pCGI, x, y, BDI_VALUE_BLANK, bdi);

			if ((y - pCGI->start_y) % (pCGI->CFI.block_high + 1) == 0 && pCGI->CFI.separator)
			{
				for (int j = 0; j < pCGI->CFI.block_width; ++j)
					cct_showstr(x + j, y, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}

			y += dir[direction][0];
			x += dir[direction][1];

			draw_block_xy(pCGI, x, y, bdi_value, bdi);
			Sleep(pCGI->delay_of_block_moved);
		}
		break;
	case DOWN_TO_UP:
		dis = distance * pCGI->CFI.block_high + (distance)*pCGI->CFI.separator;
		for (int i = 0; i < dis; ++i)
		{
			draw_block_xy(pCGI, x, y, BDI_VALUE_BLANK, bdi);

			y += dir[direction][0];
			x += dir[direction][1];

			if ((y - pCGI->start_y + pCGI->CFI.block_high) % (pCGI->CFI.block_high + 1) == 0 && pCGI->CFI.separator)
			{
				for (int j = 0; j < pCGI->CFI.block_width; ++j)
					cct_showstr(x + j, y + pCGI->CFI.block_high, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}

			draw_block_xy(pCGI, x, y, bdi_value, bdi);
			Sleep(pCGI->delay_of_block_moved);
		}
		break;
	case RIGHT_TO_LEFT:
		dis = distance * pCGI->CFI.block_width + (distance) * 2 * pCGI->CFI.separator;
		for (int i = 0; i < dis; ++i)
		{
			draw_block_xy(pCGI, x, y, BDI_VALUE_BLANK, bdi);

			y += dir[direction][0];
			x += dir[direction][1];

			if ((x - pCGI->start_x + pCGI->CFI.block_width) % (pCGI->CFI.block_width + 2) == 0 && pCGI->CFI.separator)
			{
				for (int j = 0; j < pCGI->CFI.block_high; ++j)
					cct_showstr(x + pCGI->CFI.block_width, y + j, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}

			draw_block_xy(pCGI, x, y, bdi_value, bdi);
			Sleep(pCGI->delay_of_block_moved);
		}
		break;
	case LEFT_TO_RIGHT:
		dis = distance * pCGI->CFI.block_width + (distance) * 2 * pCGI->CFI.separator;
		for (int i = 0; i < dis; ++i)
		{
			draw_block_xy(pCGI, x, y, BDI_VALUE_BLANK, bdi);

			if ((x - pCGI->start_x) % (pCGI->CFI.block_width + 2) == 0 && pCGI->CFI.separator)
			{
				for (int j = 0; j < pCGI->CFI.block_high; ++j)
					cct_showstr(x, y + j, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}

			y += dir[direction][0];
			x += dir[direction][1];

			draw_block_xy(pCGI, x, y, bdi_value, bdi);
			Sleep(pCGI->delay_of_block_moved);
		}
		break;
	default:
		return -1;
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   int &MAction							：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 三者之一
													   如果返回 CCT_KEYBOARD_EVENT，则此值无效
		   int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_ONLY_MOVED/MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列/位置非法"的信息（true=显示，false=不显示，缺省为true）
  返 回 值：函数返回约定
		   1、如果是鼠标移动，得到的MRow/MCol与传入的相同(鼠标指针微小的移动)，则不返回，继续读
							  得到行列非法位置，则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
							  得到的MRow/MCol与传入的不同(行列至少一个变化)，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"，再返回MOUSE_ONLY_MOVED（有些游戏返回后要处理色块的不同颜色显示）
		   2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
								  且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
		   3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_RIGHT_BUTTON_CLICK, MRow、MCol取当前值（因为消灭星星的右键标记需要坐标）
		   4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/

static int convert_xy_to_rc(const CONSOLE_GRAPHICS_INFO* const pCGI, int MX, int MY, int& MRow, int& MCol)
{
	int block_start_x0 = pCGI->start_x + 2;
	int block_start_y0 = pCGI->start_y + 1;//有无分界线都是一样

	for (int i = 0; i < pCGI->row_num; i++)
		for (int j = 0; j < pCGI->col_num; j++)
		{
			//给定行列(row_no,col_no)位置的色块起始坐标
			int block_start_y = block_start_y0 + i * (pCGI->CFI.block_high + 1 * pCGI->CFI.separator);
			int block_start_x = block_start_x0 + j * (pCGI->CFI.block_width + 2 * pCGI->CFI.separator);

			if (block_start_x <= MX && MX < block_start_x + pCGI->CFI.block_width &&
				block_start_y <= MY && MY < block_start_y + pCGI->CFI.block_high) 
			{
				MRow = i;
				MCol = j;
				return 0;//合法
			}
		}
	return -1;//非法
}

int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO* const pCGI, int& MAction, int& MRow,
	int& MCol, int& KeyCode1, int& KeyCode2, const bool update_lower_status_line)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;
	int MX, MY;
	while (true)
	{
		int odd_MRow = MRow;
		int odd_MCol = MCol;
		int event = cct_read_keyboard_and_mouse(MX, MY, MAction, KeyCode1, KeyCode2);
		//鼠标事件
		if (event == CCT_MOUSE_EVENT)
		{
			/*鼠标移动*/
			if (MAction == MOUSE_ONLY_MOVED)
			{
				int legal = convert_xy_to_rc(pCGI, MX, MY, MRow, MCol);
				if (legal == -1)
				{
					if (update_lower_status_line == true) //要求更新下状态栏
						gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法");
				}
				else
				{//位置合法
					if (!(odd_MRow == MRow && odd_MCol == MCol))
					{//和老坐标有变动!
						if (update_lower_status_line == true)
						{
							char display[30] = { 0 };
							char chRow = MRow + 'A';
							sprintf(display, "[当前光标] %c行%d列", chRow, MCol);
							gmw_status_line(pCGI, LOWER_STATUS_LINE, display);
						}
						return  CCT_MOUSE_EVENT;//返回
					}
					//其余情况继续循环
				}
			}
			/*鼠标左击/右击*/
			else if (MAction == MOUSE_LEFT_BUTTON_CLICK || MAction == MOUSE_RIGHT_BUTTON_CLICK)
			{
				int legal = convert_xy_to_rc(pCGI, MX, MY, MRow, MCol);
				if (legal == -1)
				{//非法
					if (update_lower_status_line == true) //要求更新下状态栏
						gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 位置非法");
				}
				else
				{//位置合法
					return  CCT_MOUSE_EVENT;//返回
				}
			}
		}

		//键盘事件
		else
		{
			return CCT_KEYBOARD_EVENT;//返回
		}
	}

}
