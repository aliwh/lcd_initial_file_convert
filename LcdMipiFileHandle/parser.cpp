//
//��������ʵ���ļ�
//
#include "parser.h"
#include <stdio.h>
#include <string.h>

enum MyEnum
{
	INNUMBER = 0,
	INCHARACTER
};

static char g_convert_buf[MAXSIZE][MAXLEN];
static char shift_buf[MAXSIZE][MAXLEN];
static int shift_n = -1;
static int shift_count = 0;

//����Ľ�������
int  parser_format0();
void parser_format1();
void parser_format2();
void parser_format3();

int parser_mipi_file(struct path* path, int parser_format)
{

	FILE *fp;
	int i = 0;

	if ((fp = fopen(path->input, "r")) == NULL) //�ж��ļ��Ƿ���ڼ��ɶ�
	{
		perror("Open file error!\n");
		return -1;
	}

	while (!feof(fp))
	{
		fgets(g_convert_buf[i], 1024, fp);  //��ȡһ��
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

	fclose(fp);                     //�ر��ļ�

	return 0;
}

int parser_format0()
{
	int i, j,n;
	char a[100];
	int state = INCHARACTER;
	FILE* fp;

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
		if (strstr(g_convert_buf[n], "LCD_ILI9881_CMD"))//����ض����ļ�
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
		perror("Create convert file error\n");//��ӡ����׼�����
		return -1;
	}

	for (i = 0; i <= shift_n; i++)
	{
		fprintf(fp, "cmd%03d = <0x39>, <1>, <0>, <2>, <0x%s>;\n", i, shift_buf[i]);// �����DTS��Ҫ��ĸ�ʽ
	}

	fclose(fp);
	
	return 0;
}

void parser_format1()
{

	return;
}

void parser_format2()
{

	return;
}

void parser_format3()
{

	return;
}


#if 0

//TODO:�ж�fp1�Ƿ�Ϊ��

for (i = 0; i <= shift_n; i++)
{
	fprintf(fp1, "cmd%03d = <0x39>, <1>, <0>, <2>, <0x%s>;\n", i, shift_buf[i]);
}
fclose(fp1);

printf("LCD mipi init code convert successed!\nSave final file in %s\n", filename);
#endif