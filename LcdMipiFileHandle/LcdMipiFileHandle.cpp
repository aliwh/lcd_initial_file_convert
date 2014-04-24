// LcdMipiFileHandle.cpp : 定义控制台应用程序的入口点。
//
//TODO:需要改进的地方
//1.只能处理H:\\ILI9881_CPT5.5_MIPI.txt这一个文件
//2.只能输出一个文件名
//3.处理的LCD命令文件格式需要更多元，更通用
//4.程度的架构需要优化下、编程规范化
//5.程序本身存在一些bug需要fix
//6.代码通过Git进行管理

#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

enum MyEnum
{
	INNUMBER = 0,
	INCHARACTER
};

static char shift_buf[1000][200];
static int shift_n = -1;
static int shift_count = 0;

int _tmain(int argc, _TCHAR* argv[])
{
	int n = 0;
	char a[1000][500];
	int i = 0, j = 0;
	int state = INCHARACTER;

	char *filename = "H:\\ILI9881_CPT5.5_MIPI.txt"; //文件名
	char commandbuf[1000][200];
	FILE *fp, *fp1;

	if ((fp = fopen(filename, "r")) == NULL) //判断文件是否存在及可读
	{
		printf("error!");
		return -1;
	}

	while (!feof(fp))
	{

		fgets(commandbuf[i], 1024, fp);  //读取一行
		i++;
	}

	fclose(fp);                     //关闭文件

	int row = i;
	for (n = 0; n < 1000; n++)
	{
		i = 0; j = 0; state = INCHARACTER;

		//提取寄存器数据
		while (commandbuf[n][i] != '\0')
		{
			if (state == INNUMBER && commandbuf[n][i] != ')' && commandbuf[n][i] != 'x' && commandbuf[n][i] != 'X' && commandbuf[n][i] != ' ')
			{
				a[n][j] = commandbuf[n][i];
				++j;
			}
			else if (commandbuf[n][i] == '0' && state == INCHARACTER)
			{
				if (commandbuf[n][i + 1] == 'x' || commandbuf[n][i + 1] == 'X')
				{
					state = INNUMBER;
				}
			}
			else if ((commandbuf[n][i] == ' ' || commandbuf[n][i] == ')') && state == INNUMBER)
			{
				state = INCHARACTER;
			}

			++i;
		}

		a[n][j] = '\0';

		//查找指定的字符串
		if (strstr(commandbuf[n], "LCD_ILI9881_CMD"))
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
				shift_buf[shift_n][shift_count++] = a[n][i];
			}

		}
		else if (strstr(commandbuf[n], "LCD_ILI9881_INDEX"))
		{
			for (i = 0; i < 2; i++)
			{
				shift_buf[shift_n][shift_count++] = a[n][i];
			}

		}
	}

	fp1 = fopen("H:\\ILI9881_CPT5.5_MIPI_shift.txt", "w");
	//TODO:判断fp1是否为空

	for (i = 0; i < shift_n; i++)
	{
		fprintf(fp1, "cmd%03d = <0x39>, <1>, <0>, <2>, <0x%s>;\n", i, shift_buf[i]);
	}
	fclose(fp1);

	printf("LCD mipi init code convert successed!\nSave final file in %s\n", filename);


	return 0;
}





