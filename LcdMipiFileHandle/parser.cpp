//
//解析器的实现文件
//
#include "parser.h"
#include <stdio.h>
#include <string.h>

enum MyEnum
{
	INNUMBER = 0,
	INCHARACTER
};

static char g_convert_buf[MAXSIZE][MAXLEN];//将文件内容读取到此数组中
static char shift_buf[MAXSIZE][MAXLEN];//转换后的数组
static int shift_n = -1;
static int shift_count = 0;

//具体的解析函数
static int  parser_format0();
static void parser_format1();
static void parser_format2();
static void parser_format3();

int parser_mipi_file(struct path* path, int parser_format)
{

	FILE *fp;
	int i = 0;

	if ((fp = fopen(path->input, "r")) == NULL) //判断文件是否存在及可读
	{
		perror("Open file error!\n");
		return -1;
	}

	while (!feof(fp))
	{
		fgets(g_convert_buf[i], 1024, fp);  //读取一行
		i++;
	}

	switch (parser_format)
	{
	case CONVERT_FORMAT0:
		parser_format0();
		break;
	case CONVERT_FORMAT1:
		parser_format1();
		break;
	case CONVERT_FORMAT2:
		parser_format2();
		break;
	default:
		printf("NO appropriate format to parser!\n");
		break;
	}

	fclose(fp);                     //关闭文件

	return 0;
}

static int parser_format0()
{
	int i, j,n;
	char a[100];
	int state = INCHARACTER;
	FILE* fp;

	for (n = 0; n < MAXSIZE; n++)
	{
		i = 0; j = 0; state = INCHARACTER;

		//利用简单状态机，提取寄存器数据
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
		if (strstr(g_convert_buf[n], "LCD_ILI9881_CMD"))//针对特定的文件
		{
			shift_n++;
			shift_count = 0;

			for (i = 0; i < 8; i++)
			{
				shift_buf[shift_n][i] = '0';
			}

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

	fp = fopen("..\\mipifiles\\ILI9881_CPT5.5_MIPI_shift.txt", "w");
	if (fp == NULL)
	{
		perror("Create convert file error\n");//打印到标准输出上
		return -1;
	}

	for (i = 0; i <= shift_n; i++)
	{
		fprintf(fp, "cmd%03d = <0x39>, <1>, <0>, <2>, <0x%s>;\n", i, shift_buf[i]);// 输出成DTS所要求的格式
	}

	fclose(fp);
	
	return 0;
}

static void parser_format1()
{

	return;
}

static void parser_format2()
{

	return;
}

static void parser_format3()
{

	return;
}
