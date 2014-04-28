// LcdMipiFileHandle.cpp : 定义控制台应用程序的入口点。
//

//TODO:需要改进的地方
//1.只能处理H:\\ILI9881_CPT5.5_MIPI.txt这一个文件
//2.只能输出一个文件名,最好在输出的文件名上增加时间，用于区分
//3.处理的LCD命令文件格式需要更多元，更通用
//4.程度的架构需要优化下、编程规范化
//5.程序本身存在一些bug需要fix（a.最后一个命令打印不出来的问题（Done:已解）;b.命令的长度问题）
//6.代码通过Git进行管理（Done:已挂靠到github服务器上）
//7.函数内数组的大小，因分配在堆栈中，不能分太大，可以考虑放到函数外或加个static关键字(不需要了，函数中不用分配那么大的数组)

#include "stdafx.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "parser.h" //lcd init 文件解析器

#define MAXSIZE 1000
#define MAXLEN 200

enum MyEnum
{
	INNUMBER = 0,
	INCHARACTER
};

static char g_convert_buf[MAXSIZE][MAXLEN];
static char shift_buf[MAXSIZE][MAXLEN];
static int shift_n = -1;
static int shift_count = 0;

//从文件中读入命令到全局数组g_shift_buf中，返回数组大小
int lcd_cmd_read(char* filename,int line)
{
	FILE *fp;
	int i = 0;

	if ((fp = fopen(filename, "r")) == NULL) //判断文件是否存在及可读
	{
		printf("error!");
		return -1;
	}

	while (!feof(fp))
	{
		
		fgets(g_convert_buf[i], line, fp);  //读取一行
		i++;
	}

	fclose(fp);                     //关闭文件

	return i;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int n = 0;
	char a[100];
	int i = 0, j = 0, row = 0;;
	int state = INCHARACTER;

	char filename[100];

#ifdef DEBUG
	char *filename = "D:\\workspace\\Mipifiles\\ILI9881_CPT5.5_MIPI.txt"; //文件名
#endif

	FILE *fp1;

	row = lcd_cmd_read(filename,1024);
	if (row < 0)
		return -1;

	for (n = 0; n < MAXSIZE; n++)
	{
		i = 0; j = 0; state = INCHARACTER;

		//提取寄存器数据
		while (g_convert_buf[n][i] != '\0')
		{
			if (state == INNUMBER && g_convert_buf[n][i] != ')' && g_convert_buf[n][i] != 'x' && g_convert_buf[n][i] != 'X' && g_convert_buf[n][i] != ' ')
			{
				a[j] = g_convert_buf[n][i];
				++j;
			}
			else if (g_convert_buf[n][i] == '0' && state == INCHARACTER)
			{
				if (g_convert_buf[n][i + 1] == 'x' || g_convert_buf[n][i + 1] == 'X')
				{
					state = INNUMBER;
				}
			}
			else if ((g_convert_buf[n][i] == ' ' || g_convert_buf[n][i] == ')') && state == INNUMBER)
			{
				state = INCHARACTER;
			}

			++i;
		}

		a[j] = '\0';


		//查找指定的字符串
		if (strstr(g_convert_buf[n], "LCD_ILI9881_CMD"))
		{
			shift_n++;
			shift_count = 0;

			for (i = 0; i < 8; i++)
			{
				shift_buf[shift_n][i] = '0';
			}

			//strcpy(shift_buf[shift_n], a[n]);
			for (i = 0; i < 2; i++)
			{
				shift_buf[shift_n][shift_count++] = a[i];
			}

		}
		else if (strstr(g_convert_buf[n], "LCD_ILI9881_INDEX"))
		{
			for (i = 0; i < 2; i++)
			{
				shift_buf[shift_n][shift_count++] = a[i];
			}

		}
		
	}

	fp1 = fopen("D:\\workspace\\Mipifiles\\ILI9881_CPT5.5_MIPI_shift.txt", "w");
	//TODO:判断fp1是否为空

	for (i = 0; i <= shift_n; i++)
	{
		fprintf(fp1, "cmd%03d = <0x39>, <1>, <0>, <2>, <0x%s>;\n", i, shift_buf[i]);
	}
	fclose(fp1);

	printf("LCD mipi init code convert successed!\nSave final file in %s\n", filename);


	return 0;
}





