// LcdMipiFileHandle.cpp : �������̨Ӧ�ó������ڵ㡣
//

//TODO:��Ҫ�Ľ��ĵط�
//1.ֻ�ܴ���H:\\ILI9881_CPT5.5_MIPI.txt��һ���ļ�
//2.ֻ�����һ���ļ���,�����������ļ���������ʱ�䣬��������
//3.�����LCD�����ļ���ʽ��Ҫ����Ԫ����ͨ��
//4.�̶ȵļܹ���Ҫ�Ż��¡���̹淶��
//5.���������һЩbug��Ҫfix��a.���һ�������ӡ�����������⣨Done:�ѽ⣩;b.����ĳ������⣩
//6.����ͨ��Git���й���Done:�ѹҿ���github�������ϣ�
//7.����������Ĵ�С��������ڶ�ջ�У����ܷ�̫�󣬿��Կ��Ƿŵ��������Ӹ�static�ؼ���(����Ҫ�ˣ������в��÷�����ô�������)

#include "stdafx.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "parser.h" //lcd init �ļ�������

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

//���ļ��ж������ȫ������g_shift_buf�У����������С
int lcd_cmd_read(char* filename,int line)
{
	FILE *fp;
	int i = 0;

	if ((fp = fopen(filename, "r")) == NULL) //�ж��ļ��Ƿ���ڼ��ɶ�
	{
		printf("error!");
		return -1;
	}

	while (!feof(fp))
	{
		
		fgets(g_convert_buf[i], line, fp);  //��ȡһ��
		i++;
	}

	fclose(fp);                     //�ر��ļ�

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
	char *filename = "D:\\workspace\\Mipifiles\\ILI9881_CPT5.5_MIPI.txt"; //�ļ���
#endif

	FILE *fp1;

	row = lcd_cmd_read(filename,1024);
	if (row < 0)
		return -1;

	for (n = 0; n < MAXSIZE; n++)
	{
		i = 0; j = 0; state = INCHARACTER;

		//��ȡ�Ĵ�������
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


		//����ָ�����ַ���
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
	//TODO:�ж�fp1�Ƿ�Ϊ��

	for (i = 0; i <= shift_n; i++)
	{
		fprintf(fp1, "cmd%03d = <0x39>, <1>, <0>, <2>, <0x%s>;\n", i, shift_buf[i]);
	}
	fclose(fp1);

	printf("LCD mipi init code convert successed!\nSave final file in %s\n", filename);


	return 0;
}





