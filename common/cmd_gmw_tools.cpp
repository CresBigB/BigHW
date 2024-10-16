/* 学号 姓名 班级 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
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
	// 防止在未调用 gmw_init 前调用其它函数
	if (pCGI->inited != CGI_INITED)
		return -1;

	// 检查行列数的有效性
	if (row <= 0 || col <= 0)
		return -2; // 返回错误代码，表示行数或列数不合法

	// 更新行列数
	pCGI->row_num = row;
	pCGI->col_num = col;

	// 更新与行列数相关的其他成员值
	// 计算游戏区域的宽度和高度
	int frameWidth = col * pCGI->CFI.block_width; // 计算游戏主框架区域的宽度
	int frameHeight = row * pCGI->CFI.block_high; // 计算游戏主框架区域的高度

	// 根据行列数更新主框架区域的参考坐标
	pCGI->start_x = pCGI->extern_left_cols; // 参考坐标的 X 轴位置
	pCGI->start_y = pCGI->extern_up_lines + (pCGI->top_status_line ? 1 : 0); // 参考坐标的 Y 轴位置

	// 重新计算 cmd 窗口的大小
	pCGI->lines = frameHeight + pCGI->extern_up_lines + pCGI->extern_down_lines + (pCGI->top_status_line ? 1 : 0) + (pCGI->lower_status_line ? 1 : 0) + 4;
	pCGI->cols = frameWidth + pCGI->extern_left_cols + pCGI->extern_right_cols;

	// 更新主框架的附加宽度和高度
	if (pCGI->CFI.separator)
	{
		pCGI->CFI.block_width_ext = pCGI->CFI.block_width + 1; // 考虑分隔线的宽度
		pCGI->CFI.block_high_ext = pCGI->CFI.block_high; // 分隔线的高度为1
	}
	else
	{
		pCGI->CFI.block_width_ext = pCGI->CFI.block_width; // 无分隔线时
		pCGI->CFI.block_high_ext = pCGI->CFI.block_high; // 高度不变
	}

	return 0; // 成功
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
// 设置整个窗口的颜色
int gmw_set_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	// 防止在未调用 gmw_init 前调用其它函数
	if (pCGI->inited != CGI_INITED)
		return -1;

	// 设置整个窗口的颜色
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;

	if (cascade)
	{
		// 同步设置游戏主区域的颜色
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;

		// 同步设置状态栏的颜色
		if (pCGI->SLI.is_top_status_line)
		{
			pCGI->SLI.top_normal_bgcolor = bgcolor;
			pCGI->SLI.top_normal_fgcolor = fgcolor;
			// 这里可以设置醒目信息的背景色，前景色保持不变
			pCGI->SLI.top_catchy_bgcolor = bgcolor;
		}

		if (pCGI->SLI.is_lower_status_line)
		{
			pCGI->SLI.lower_normal_bgcolor = bgcolor;
			pCGI->SLI.lower_normal_fgcolor = fgcolor;
			// 这里可以设置醒目信息的背景色，前景色保持不变
			pCGI->SLI.lower_catchy_bgcolor = bgcolor;
		}
	}

	return 0; // 成功
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
	if (pCGI == nullptr || pCGI->inited != CGI_INITED) // 检查指针是否有效
		return -1; // 返回错误代码

	/* 设置字体名称和大小 */
	// 确保字体名称不超过 FONTNAME_LEN - 1
	strncpy(pCGI->CFT.font_type, fontname, FONTNAME_LEN - 1);
	pCGI->CFT.font_type[FONTNAME_LEN] = '\0'; // 确保字符串以零结尾

	// 设置字体的宽度和高度
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;

	return 0; // 成功设置字体，返回0
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
	if (pCGI == nullptr || pCGI->inited != CGI_INITED) // 检查指针是否有效
		return -1; // 返回错误代码

	// 根据延时类型设置延时
	switch (type)
	{
	case DELAY_OF_DRAW_FRAME: // 画边框的延时
		if (delay_ms < 0)
			pCGI->delay_of_draw_frame = 0; // 置为0
		else
			pCGI->delay_of_draw_frame = delay_ms; // 设置延时
		break;

	case DELAY_OF_DRAW_BLOCK: // 画色块的延时
		if (delay_ms < 0)
			pCGI->delay_of_draw_block = 0; // 置为0
		else
			pCGI->delay_of_draw_block = delay_ms; // 设置延时
		break;

	case DELAY_OF_BLOCK_MOVED: // 色块移动的延时
		if (delay_ms < BLOCK_MOVED_DELAY_MS)
			pCGI->delay_of_block_moved = BLOCK_MOVED_DELAY_MS; // 置为BLOCK_MOVED_DELAY_MS
		else
			pCGI->delay_of_block_moved = delay_ms; // 设置延时
		break;

	default:
		return -1; // 如果类型不合法，返回错误代码
	}

	return 0; // 成功设置延时，返回0
}


/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines				：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO* const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI == nullptr || pCGI->inited != CGI_INITED) // 检查指针是否有效
		return -1; // 返回错误代码

	// 处理上部额外行数
	if (up_lines < 0)
		pCGI->extern_up_lines = 0; // 置为0
	else
		pCGI->extern_up_lines = up_lines; // 设置上部额外行数

	// 处理下部额外行数
	if (down_lines < 0)
		pCGI->extern_down_lines = 0; // 置为0
	else
		pCGI->extern_down_lines = down_lines; // 设置下部额外行数

	// 处理左边额外列数
	if (left_cols < 0)
		pCGI->extern_left_cols = 0; // 置为0
	else
		pCGI->extern_left_cols = left_cols; // 设置左边额外列数

	// 处理右边额外列数
	if (right_cols < 0)
		pCGI->extern_right_cols = 0; // 置为0
	else
		pCGI->extern_right_cols = right_cols; // 设置右边额外列数

	// 更新其他成员，确保额外区域的变化反映在整体结构中
	pCGI->row_num += pCGI->extern_up_lines + pCGI->extern_down_lines; // 更新总高度
	pCGI->col_num = pCGI->extern_left_cols + pCGI->extern_right_cols; // 更新总宽度

	return 0; // 成功设置额外行列，返回0
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
	switch (type) {
	case 1: // 全线
		strncpy(pCGI->CFI.top_left, "╔", CFI_LEN);
		strncpy(pCGI->CFI.lower_left, "╚", CFI_LEN);
		strncpy(pCGI->CFI.top_right, "╗", CFI_LEN);
		strncpy(pCGI->CFI.lower_right, "╝", CFI_LEN);
		strncpy(pCGI->CFI.h_normal, "═", CFI_LEN);
		strncpy(pCGI->CFI.v_normal, "║", CFI_LEN);
		strncpy(pCGI->CFI.h_top_separator, "╤", CFI_LEN);
		strncpy(pCGI->CFI.h_lower_separator, "╧", CFI_LEN);
		strncpy(pCGI->CFI.v_left_separator, "╟", CFI_LEN);
		strncpy(pCGI->CFI.v_right_separator, "╢", CFI_LEN);
		strncpy(pCGI->CFI.mid_separator, "┼", CFI_LEN);
		break;

	case 2: // 全单线
		strncpy(pCGI->CFI.top_left, "+", CFI_LEN);
		strncpy(pCGI->CFI.lower_left, "+", CFI_LEN);
		strncpy(pCGI->CFI.top_right, "+", CFI_LEN);
		strncpy(pCGI->CFI.lower_right, "+", CFI_LEN);
		strncpy(pCGI->CFI.h_normal, "-", CFI_LEN);
		strncpy(pCGI->CFI.v_normal, "|", CFI_LEN);
		strncpy(pCGI->CFI.h_top_separator, "+", CFI_LEN);
		strncpy(pCGI->CFI.h_lower_separator, "+", CFI_LEN);
		strncpy(pCGI->CFI.v_left_separator, "+", CFI_LEN);
		strncpy(pCGI->CFI.v_right_separator, "+", CFI_LEN);
		strncpy(pCGI->CFI.mid_separator, "+", CFI_LEN);
		break;

	case 3: // 横双竖单
		strncpy(pCGI->CFI.top_left, "╔", CFI_LEN);
		strncpy(pCGI->CFI.lower_left, "╚", CFI_LEN);
		strncpy(pCGI->CFI.top_right, "╗", CFI_LEN);
		strncpy(pCGI->CFI.lower_right, "╝", CFI_LEN);
		strncpy(pCGI->CFI.h_normal, "═", CFI_LEN);
		strncpy(pCGI->CFI.v_normal, "│", CFI_LEN); // 使用单竖线
		strncpy(pCGI->CFI.h_top_separator, "╤", CFI_LEN);
		strncpy(pCGI->CFI.h_lower_separator, "╧", CFI_LEN);
		strncpy(pCGI->CFI.v_left_separator, "╟", CFI_LEN);
		strncpy(pCGI->CFI.v_right_separator, "╢", CFI_LEN);
		strncpy(pCGI->CFI.mid_separator, "┼", CFI_LEN);
		break;

	case 4: // 横单竖双
		strncpy(pCGI->CFI.top_left, "╔", CFI_LEN);
		strncpy(pCGI->CFI.lower_left, "╚", CFI_LEN);
		strncpy(pCGI->CFI.top_right, "╗", CFI_LEN);
		strncpy(pCGI->CFI.lower_right, "╝", CFI_LEN);
		strncpy(pCGI->CFI.h_normal, "═", CFI_LEN);
		strncpy(pCGI->CFI.v_normal, "║", CFI_LEN); // 使用双竖线
		strncpy(pCGI->CFI.h_top_separator, "╤", CFI_LEN);
		strncpy(pCGI->CFI.h_lower_separator, "╧", CFI_LEN);
		strncpy(pCGI->CFI.v_left_separator, "╟", CFI_LEN);
		strncpy(pCGI->CFI.v_right_separator, "╢", CFI_LEN);
		strncpy(pCGI->CFI.mid_separator, "┼", CFI_LEN);
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
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left, const char* top_right,
	const char* lower_right, const char* h_normal, const char* v_normal, const char* h_top_separator,
	const char* h_lower_separator, const char* v_left_separator, const char* v_right_separator, const char* mid_separator)
{
	// 防止在未调用 gmw_init 前调用其它函数
	if (pCGI->inited != CGI_INITED)
		return -1;

	auto set_line_type = [](char* dest, const char* src) {
		if (src == nullptr) {
			// 如果给NULL，用两个空格替代
			strncpy(dest, "  ", CFI_LEN);
		}
		else {
			// 处理输入长度
			int len = strlen(src);
			if (len == 1) {
				// 如果给1字节，则补一个空格
				strncpy(dest, src, 1);
				dest[1] = ' ';
			}
			else if (len > 2) {
				// 超过2字节则只取前2字节
				strncpy(dest, src, 2);
			}
			else {
				// 正常复制
				strncpy(dest, src, len);
			}
			// 确保字符串以空字符结尾
			dest[CFI_LEN - 1] = '\0';
		}
		};

	// 设置各个边框和线型
	set_line_type(pCGI->CFI.top_left, top_left);
	set_line_type(pCGI->CFI.lower_left, lower_left);
	set_line_type(pCGI->CFI.top_right, top_right);
	set_line_type(pCGI->CFI.lower_right, lower_right);
	set_line_type(pCGI->CFI.h_normal, h_normal);
	set_line_type(pCGI->CFI.v_normal, v_normal);
	set_line_type(pCGI->CFI.h_top_separator, h_top_separator);
	set_line_type(pCGI->CFI.h_lower_separator, h_lower_separator);
	set_line_type(pCGI->CFI.v_left_separator, v_left_separator);
	set_line_type(pCGI->CFI.v_right_separator, v_right_separator);
	set_line_type(pCGI->CFI.mid_separator, mid_separator);

	return 0; // 设置成功
}
/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
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

	// 设置色块的宽度和高度
	int adjusted_block_width = (block_width % 2 == 1) ? block_width + 1 : block_width; // 如果是奇数，则+1
	pCGI->CFI.block_width = adjusted_block_width;
	pCGI->CFI.block_high = block_high > 0 ? block_high : 1; // 确保高度至少为1

	// 设置是否需要分隔线
	pCGI->CFI.separator = separator;

	// 根据 block_width 和 separator 计算附加宽度和高度
	if (separator)
	{
		pCGI->CFI.block_width_ext = adjusted_block_width + 2; // 分隔线的情况下，额外加2
		pCGI->CFI.block_high_ext = block_high + 1; // 分隔线的情况下，额外加1
	}
	else
	{
		pCGI->CFI.block_width_ext = adjusted_block_width; // 无分隔线
		pCGI->CFI.block_high_ext = block_high; // 无分隔线
	}

	// 根据 block_width 和 block_high 更新总宽度和高度
	pCGI->CFI.bwidth = adjusted_block_width + (separator ? 1 : 0); // 每行含分隔线的总宽度
	pCGI->CFI.bhigh = block_high + (separator ? 1 : 0); // 每列含分隔线的总高度

	// 计算状态栏的宽度（假设游戏区域宽度由其他因素影响，计算方法依赖具体实现）
	pCGI->CFI.bwidth = pCGI->col_num * adjusted_block_width + (separator ? (pCGI->col_num - 1) : 0);

	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + pCGI->CFI.block_high_ext;
	pCGI->cols = pCGI->CFI.block_width * pCGI->col_num + pCGI->CFI.block_width_ext;

	return 0; // 函数成功完成
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

	// 如果背景色为-1，使用窗口的背景色
	if (bgcolor == -1)
	{
		pCGI->area_bgcolor = pCGI->area_bgcolor; // 假设有一个窗口背景色的成员
	}
	else
	{
		pCGI->area_bgcolor = bgcolor; // 否则使用传入的背景色
	}

	// 如果前景色为-1，使用窗口的前景色
	if (fgcolor == -1)
	{
		pCGI->area_fgcolor = pCGI->area_fgcolor; // 假设有一个窗口前景色的成员
	}
	else
	{
		pCGI->area_fgcolor = fgcolor; // 否则使用传入的前景色
	}

	return 0; // 函数成功完成
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
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	// 根据 type 设置线型
	switch (type) {
	case 1: // 全双线
		// 填充双线信息
		memcpy(pCGI->CBI.top_left, "╔", CBI_LEN);
		memcpy(pCGI->CBI.lower_left, "╚", CBI_LEN);
		memcpy(pCGI->CBI.top_right, "╗", CBI_LEN);
		memcpy(pCGI->CBI.lower_right, "╝", CBI_LEN);
		memcpy(pCGI->CBI.h_normal, "═", CBI_LEN);
		memcpy(pCGI->CBI.v_normal, "║", CBI_LEN);
		break;

	case 2: // 全单线
		// 填充单线信息
		memcpy(pCGI->CBI.top_left, "┌", CBI_LEN);
		memcpy(pCGI->CBI.lower_left, "└", CBI_LEN);
		memcpy(pCGI->CBI.top_right, "┐", CBI_LEN);
		memcpy(pCGI->CBI.lower_right, "┘", CBI_LEN);
		memcpy(pCGI->CBI.h_normal, "─", CBI_LEN);
		memcpy(pCGI->CBI.v_normal, "│", CBI_LEN);
		break;

	case 3: // 横双竖单
		memcpy(pCGI->CBI.top_left, "╔", CBI_LEN);
		memcpy(pCGI->CBI.lower_left, "╚", CBI_LEN);
		memcpy(pCGI->CBI.top_right, "╗", CBI_LEN);
		memcpy(pCGI->CBI.lower_right, "╝", CBI_LEN);
		memcpy(pCGI->CBI.h_normal, "═", CBI_LEN);
		memcpy(pCGI->CBI.v_normal, "│", CBI_LEN);
		break;

	case 4: // 横单竖双
		memcpy(pCGI->CBI.top_left, "┌", CBI_LEN);
		memcpy(pCGI->CBI.lower_left, "└", CBI_LEN);
		memcpy(pCGI->CBI.top_right, "┐", CBI_LEN);
		memcpy(pCGI->CBI.lower_right, "┘", CBI_LEN);
		memcpy(pCGI->CBI.h_normal, "─", CBI_LEN);
		memcpy(pCGI->CBI.v_normal, "║", CBI_LEN);
		break;

	default:
		return -1; // 不支持的线型
	}

	return 0; // 返回成功
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

// 填充函数以辅助处理线型
static void set_line_type(const char* input, char* output)
{
	if (input == NULL)
	{
		// 用两个空格替代
		output[0] = ' ';
		output[1] = ' ';
	}
	else
	{
		size_t len = strlen(input);
		if (len >= 2)
		{
			// 取前2字节
			memcpy(output, input, 2);
		}
		else
		{
			// 处理1字节的情况
			memcpy(output, input, len);
			output[len] = ' '; // 补一个空格
		}
		// 确保数组末尾为空字符
		//output[2 - 1] = '\0'; // 保证以'\0'结尾
	}
}

int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO* const pCGI, const char* top_left, const char* lower_left, const char* top_right, const char* lower_right, const char* h_normal, const char* v_normal)
{
	// 防止在未调用 gmw_init 前调用其它函数
	if (pCGI->inited != CGI_INITED)
		return -1;

	// 定义用于存储线型的字符数组，长度为2
	char line_type[2];

	// 设置每种线型
	set_line_type(top_left, line_type);
	memcpy(pCGI->CBI.top_left, line_type, 2);

	set_line_type(lower_left, line_type);
	memcpy(pCGI->CBI.lower_left, line_type, 2);

	set_line_type(top_right, line_type);
	memcpy(pCGI->CBI.top_right, line_type, 2);

	set_line_type(lower_right, line_type);
	memcpy(pCGI->CBI.lower_right, line_type, 2);

	set_line_type(h_normal, line_type);
	memcpy(pCGI->CBI.h_normal, line_type, 2);

	set_line_type(v_normal, line_type);
	memcpy(pCGI->CBI.v_normal, line_type, 2);

	return 0; // 成功返回0
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off					：true - 需要 false - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	// 根据 on_off 参数设置边框状态
	pCGI->CBI.block_border = on_off;

	// 如果需要边框，设置相应的线型为双线
	if (on_off) {
		const char* double_line_top_left = "╔";   // 示例字符，实际使用时根据需要修改
		const char* double_line_lower_left = "╚"; // 示例字符
		const char* double_line_top_right = "╗";  // 示例字符
		const char* double_line_lower_right = "╝"; // 示例字符
		const char* double_line_h_normal = "═";    // 示例字符
		const char* double_line_v_normal = "║";    // 示例字符

		// 使用 memcpy 复制双线字符
		memcpy(pCGI->CBI.top_left, double_line_top_left, 2);
		memcpy(pCGI->CBI.lower_left, double_line_lower_left, 2);
		memcpy(pCGI->CBI.top_right, double_line_top_right, 2);
		memcpy(pCGI->CBI.lower_right, double_line_lower_right, 2);
		memcpy(pCGI->CBI.h_normal, double_line_h_normal, 2);
		memcpy(pCGI->CBI.v_normal, double_line_v_normal, 2);
	}
	else {
		const char* empty_line = "  "; // 空字符

		// 使用 memcpy 设置为空字符
		memcpy(pCGI->CBI.top_left, empty_line, 2);
		memcpy(pCGI->CBI.lower_left, empty_line, 2);
		memcpy(pCGI->CBI.top_right, empty_line, 2);
		memcpy(pCGI->CBI.lower_right, empty_line, 2);
		memcpy(pCGI->CBI.h_normal, empty_line, 2);
		memcpy(pCGI->CBI.v_normal, empty_line, 2);
	}

	return 0; // 成功返回0
}


/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const bool on_off					：true - 需要 false - 不需要（缺省true）
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

	// 根据 type 设置状态栏的开关
	if (type == TOP_STATUS_LINE)
	{ // 上状态栏
		pCGI->SLI.is_top_status_line = on_off; // 根据需要更新结构体成员
		if (!on_off)
			pCGI->col_num -= pCGI->extern_up_lines;

	}
	else if (type == LOWER_STATUS_LINE)
	{ // 下状态栏
		pCGI->SLI.is_lower_status_line = on_off; // 根据需要更新结构体成员
		if (!on_off)
			pCGI->col_num -= pCGI->extern_down_lines;
	}
	else
	{
		return -1; // 无效的类型参数，返回错误
	}

	return 0; // 成功返回0
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
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO* const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	// 根据状态栏类型设置颜色
	if (type == 1)
	{ // 上状态栏
		// 设置正常文本颜色
		pCGI->SLI.top_catchy_bgcolor = (normal_bgcolor == -1) ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.top_catchy_fgcolor = (normal_fgcolor == -1) ? pCGI->area_fgcolor : normal_fgcolor;

		// 设置醒目文本颜色
		pCGI->SLI.top_catchy_bgcolor = (catchy_bgcolor == -1) ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.top_catchy_fgcolor = (catchy_fgcolor == -1) ? pCGI->area_fgcolor : catchy_fgcolor;

	}
	else if (type == 2)
	{ // 下状态栏
		// 设置正常文本颜色
		pCGI->SLI.lower_normal_bgcolor = (normal_bgcolor == -1) ? pCGI->area_bgcolor : normal_bgcolor;
		pCGI->SLI.lower_normal_fgcolor = (normal_fgcolor == -1) ? pCGI->area_fgcolor : normal_fgcolor;

		// 设置醒目文本颜色
		pCGI->SLI.lower_catchy_bgcolor = (catchy_bgcolor == -1) ? pCGI->area_bgcolor : catchy_bgcolor;
		pCGI->SLI.lower_catchy_fgcolor = (catchy_fgcolor == -1) ? pCGI->area_fgcolor : catchy_fgcolor;

	}
	else
	{
		return -1; // 无效的状态栏类型
	}

	return 0; // 成功返回
}


/***************************************************************************
  函数名称：gmw_set_rowno_switch
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：int									：返回状态，0表示成功，-1表示错误
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
			2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO* const pCGI, const bool on_off)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	// 更新行号显示状态
	pCGI->draw_frame_with_row_no = on_off;

	// 根据行号显示状态处理其它成员的变化
	if (on_off)
	{
		// 如果开启显示行号，更新与行号相关的宽度、列号等属性
		pCGI->row_num += CBI_LEN; // 假设定义了一个行号宽度常量
	}

	// 根据需要更新其他可能受影响的成员
	// 比如可能需要更新整体布局、显示区域等，具体视项目需求而定

	return 0; // 成功返回
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
	// 更新行号显示状态
	pCGI->draw_frame_with_col_no = on_off;

	// 根据行号显示状态处理其它成员的变化
	if (on_off)
	{
		// 如果开启显示行号，更新与行号相关的宽度、列号等属性
		pCGI->col_num += CBI_LEN; // 假设定义了一个行号宽度常量
	}

	// 根据需要更新其他可能受影响的成员
	// 比如可能需要更新整体布局、显示区域等，具体视项目需求而定

	return 0; // 成功返回
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
  函数名称：gmw_init
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：int								：返回 0 表示初始化成功
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO* const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	/* 首先置标记，表示已初始化 */
	pCGI->inited = CGI_INITED;

	/* 初始化主框架信息 */
	gmw_set_frame_style(pCGI);
	gmw_set_frame_color(pCGI, bgcolor, fgcolor);
	gmw_set_frame_linetype(pCGI);
	//memcpy(pCGI->CFI.top_left, "╔", CFI_LEN);
	//memcpy(pCGI->CFI.lower_left, "╚", CFI_LEN);
	//memcpy(pCGI->CFI.top_right, "╗", CFI_LEN);
	//memcpy(pCGI->CFI.lower_right, "╝", CFI_LEN);
	//memcpy(pCGI->CFI.h_normal, "═", CFI_LEN);
	//memcpy(pCGI->CFI.v_normal, "║", CFI_LEN);
	//memcpy(pCGI->CFI.h_top_separator, "╤", CFI_LEN);
	//memcpy(pCGI->CFI.h_lower_separator, "╧", CFI_LEN);
	//memcpy(pCGI->CFI.v_left_separator, "╟", CFI_LEN);
	//memcpy(pCGI->CFI.v_right_separator, "╢", CFI_LEN);
	//memcpy(pCGI->CFI.mid_separator, "┼", CFI_LEN);
	//pCGI->CFI.bgcolor = bgcolor;
	//pCGI->CFI.fgcolor = fgcolor;
	//pCGI->CFI.block_width = 2;   // 色块宽度必须为2的倍数
	//pCGI->CFI.block_high = 1;    // 色块高度
	//pCGI->CFI.separator = false; // 默认不需要分隔线

	/* 初始化色块信息 */
	gmw_set_color(pCGI, bgcolor, fgcolor);
	gmw_set_block_border_switch(pCGI);
	//memcpy(pCGI->CBI.top_left, "╔", CBI_LEN);
	//memcpy(pCGI->CBI.lower_left, "╚", CBI_LEN);
	//memcpy(pCGI->CBI.top_right, "╗", CBI_LEN);
	//memcpy(pCGI->CBI.lower_right, "╝", CBI_LEN);
	//memcpy(pCGI->CBI.h_normal, "═", CBI_LEN);
	//memcpy(pCGI->CBI.v_normal, "║", CBI_LEN);
	//pCGI->CBI.block_border = false;  // 默认无小边框

	/* 初始化状态栏信息 */
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE);
	gmw_set_status_line_color(pCGI, TOP_STATUS_LINE, bgcolor, fgcolor);
	gmw_set_status_line_color(pCGI, LOWER_STATUS_LINE, bgcolor, fgcolor);
	//pCGI->SLI.is_top_status_line = true;    // 默认有上状态栏
	//pCGI->SLI.is_lower_status_line = true;  // 默认有下状态栏
	//pCGI->SLI.top_start_x = 0;
	//pCGI->SLI.top_start_y = 0;
	//pCGI->SLI.top_normal_bgcolor = COLOR_BLACK;  // 默认上状态栏背景色
	//pCGI->SLI.top_normal_fgcolor = COLOR_WHITE;  // 默认上状态栏前景色
	//pCGI->SLI.top_catchy_bgcolor = COLOR_BLACK;  // 默认醒目文本背景色
	//pCGI->SLI.top_catchy_fgcolor = COLOR_YELLOW; // 默认醒目文本前景色
	//pCGI->SLI.lower_start_x = 0;
	//pCGI->SLI.lower_start_y = row - 1;          // 下状态栏放在最后一行
	//pCGI->SLI.lower_normal_bgcolor = COLOR_BLACK;
	//pCGI->SLI.lower_normal_fgcolor = COLOR_WHITE;
	//pCGI->SLI.lower_catchy_bgcolor = COLOR_BLACK;
	//pCGI->SLI.lower_catchy_fgcolor = COLOR_YELLOW;

	/* 初始化字体信息 */
	gmw_set_font(pCGI);
	//strcpy(pCGI->CFT.font_type, "default");  // 默认字体
	//pCGI->CFT.font_size_width = 16;
	//pCGI->CFT.font_size_high = 8;

	/* 初始化图形区域设置 */
	gmw_set_rowno_switch(pCGI);
	gmw_set_colno_switch(pCGI);
	pCGI->extern_up_lines = 0;
	pCGI->extern_down_lines = 0;
	pCGI->extern_left_cols = 0;
	pCGI->extern_right_cols = 0;
	pCGI->row_num = row * pCGI->CFI.block_high * 2;			// 色块行数
	pCGI->col_num = col * pCGI->CFI.block_width * 2;		// 色块列数
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	//pCGI->top_status_line = true;
	//pCGI->lower_status_line = true;
	//pCGI->draw_frame_with_row_no = false;  // 默认不显示行号
	//pCGI->draw_frame_with_col_no = false;  // 默认不显示列标

	/* 初始化延时设置 */
	pCGI->delay_of_draw_frame = 0;    // 无延时
	pCGI->delay_of_draw_block = 0;    // 无延时
	pCGI->delay_of_block_moved = 0;   // 无延时

	/* 设置起始坐标 */
	pCGI->start_x = 0;
	pCGI->start_y = 0;

	/* 设置控制台窗口大小 */
	pCGI->lines = row + 8;  // 包含上下状态栏和一些预留行
	pCGI->cols = col + 4;
	return 0; // 初始化成功
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
	for (int i = pCGI->start_y; i <= pCGI->lines + pCGI->start_y; i += 1)//pCGI->CFI.block_high)
	{
		for (int j = pCGI->start_x; j <= pCGI->start_x + pCGI->cols; j += 1)//pCGI->CFI.block_width)
		{
			//左上
			if (i == pCGI->start_y && j == pCGI->start_x)
				cct_showstr(j, i, pCGI->CFI.top_left, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			//右上
			else if (j <= pCGI->start_x + pCGI->cols && i == pCGI->start_y)
				cct_showstr(j, i, pCGI->CFI.top_right, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			//左下
			else if (i == pCGI->lines + pCGI->start_y && j == pCGI->start_x)
				cct_showstr(j, i, pCGI->CFI.lower_left, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			//右下
			else if (i == pCGI->lines + pCGI->start_y && j == pCGI->start_x + pCGI->cols)
				cct_showstr(j, i, pCGI->CFI.lower_right, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			//最上下横线
			else if (i == pCGI->start_y || i == pCGI->lines + pCGI->start_y)
				cct_showstr(j, i, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			//最左右竖线
			else if (j == 0 || j == pCGI->start_x + pCGI->cols)
				cct_showstr(j, i, pCGI->CFI.v_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);

			if (pCGI->CFI.separator && j != pCGI->start_x && j != pCGI->lines)
			{
				//最上方分割线
				if (i == pCGI->start_y && j != pCGI->start_x && j != pCGI->start_x + pCGI->cols && j % pCGI->CFI.block_width == 0)
					cct_showstr(j, i, pCGI->CFI.h_top_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				//最下方分割线
				else if (i == pCGI->lines + pCGI->start_y && j != pCGI->start_x
					&& j != pCGI->start_x + pCGI->cols && j % pCGI->CFI.block_width == 0)
					cct_showstr(j, i, pCGI->CFI.h_lower_separator, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}
		}
	}

	return 0; //此句可根据需要修改
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

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
			2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO* const bdi)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

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
int gmw_move_block(const CONSOLE_GRAPHICS_INFO* const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO* const bdi, const int direction, const int distance)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

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
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO* const pCGI, int& MAction, int& MRow, int& MCol, int& KeyCode1, int& KeyCode2, const bool update_lower_status_line)
{
	/* 防止在未调用 gmw_init 前调用其它函数 */
	if (pCGI->inited != CGI_INITED)
		return -1;

	return 0; //此句可根据需要修改
}
