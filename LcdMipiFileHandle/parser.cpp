//
//��������ʵ���ļ�
//
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum MyEnum
{
	INNUMBER = 0,
	INCHARACTER
};

static char g_convert_buf[MAXSIZE][MAXLEN];//���ļ����ݶ�ȡ����������
static char shift_buf[MAXSIZE][MAXLEN];//ת���������
static int shift_n = -1;
static int shift_count = 0;

//����Ľ�������
static int  parser_format0();
static int parser_format1();
static void parser_format2();
static void parser_format3();

int parser_mipi_file(struct path* path, int parser_format)
{

	FILE *fp;
	int i = 0;

	if ((fp = fopen(path->input, "r")) == NULL) //�ж��ļ��Ƿ���ڼ��ɶ�,��TODO:���·�����󣬿�����Ĭ�ϵ��ļ���)
	{
		perror("Open file error!\n");
		return -1;
	}

	while (!feof(fp))
	{
		fgets(g_convert_buf[i], 1024, fp);  //��ȡһ��
		i++;
	}
	//�Ľ���ʹ�ûص�����
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

static int parser_format0()
{
	int i, j,n;
	char a[100];
	int state = INCHARACTER;
	FILE* fp;

	for (n = 0; n < MAXSIZE; n++)
	{
		i = 0; j = 0; state = INCHARACTER;

		//���ü�״̬������ȡ�Ĵ�������
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


//�Դ���Ĵ������ַ������и�ʽ��
static int data_format(char* src, char* dest)
{
	size_t i = 0, j = 0;
	
	while (*src != '\0')
	{
		if (i% 8 == 0)
		{
			if (i == 0)
			{
				*dest++ = '0';
				*dest++ = 'x';
			}
			else
			{
				*dest++ = ' ';
				*dest++ = '0';
				*dest++ = 'x';
			}

		}
		*dest++ = *src++;
		i++;
	}
	
	printf("%d,%d\n", i,i%8);
	j = 8 - i % 8 ;

	if (j != 8)
	{
		while (j-- != 0)
		{
			*dest++ = '0';
		}
	}

	*dest = '\0';
	return i/2;
}


//  ������ģ�鹩Ӧ���ṩ��LCD ��ʼ���ļ�
//  ֧�ֵĽ�����ʽ�磺
//  ...
//  W_COM_1A_1P(0x00,0x80);
//  W_COM_1A_6P(0xCE, 0x89, 0x03, 0x1C, 0x88, 0x03, 0x1C);
//  W_COM_1A_1P(0x00, 0xA0);
//  W_COM_1A_14P(0xCE, 0x38, 0x07, 0x03, 0x5C, 0x00, 0x1C, 0x00, 0x38, 0x04, 0x03, 0x57, 0x00, 0x1C, 0x00);
//  W_COM_1A_1P(0x00, 0xB0);
//  W_COM_1A_14P(0xCE, 0x38, 0x05, 0x03, 0x56, 0x00, 0x1C, 0x00, 0x38, 0x06, 0x03, 0x5F, 0x00, 0x1C, 0x00);
//   ...
// ������ARM Kernel DTS֧�ֵĸ�ʽ����
//   ...
//  cmd032 = <0x39>, <1>, <0>, <2>, <0x00800000>;
//  cmd033 = <0x39>, <1>, <0>, <7>, <0xCE89031C 0x88031C00>;
//  cmd034 = <0x39>, <1>, <0>, <2>, <0x00A00000>;
//  cmd035 = <0x39>, <1>, <0>, <15>, <0xCE380703 0x5C001C00 0x38040357 0x001C0000>;
//  cmd036 = <0x39>, <1>, <0>, <2>, <0x00B00000>;
//  cmd037 = <0x39>, <1>, <0>, <15>, <0xCE380503 0x56001C00 0x3806035F 0x001C0000>;
//   ...
static int parser_format1()
{
	FILE* fp;
	char c;
	enum MyEnum
	{
		STAT_NONE,
		STAT_TEXT,
		STAT_START_TAG,
		STAT_PRE_COMMAND,
		STAT_COMMAND,
	}state = STAT_NONE;
	char tmp[100];
	char fdata[100];

	fp = fopen("..\\mipifiles\\OTM8019A_shift.txt", "w");
	if (fp == NULL)
	{
		perror("Create convert file error\n");//��ӡ����׼�����
		return -1;
	}

	for (size_t i = 0; i < MAXSIZE; i++)
	{
		size_t n = 0,j = 0;
		state = STAT_NONE;
	
		while ((c = g_convert_buf[i][n]) != '\0' && (c != ';'))
		{
			switch (state)
			{
			case STAT_NONE:
				if (isalpha(c))
				{
					state = STAT_TEXT;
				}
				break;
			case STAT_TEXT:
				if (c == '(')
				{
					state = STAT_START_TAG;
				}
				break;
			case STAT_START_TAG:
				if (c == '0')
				{
					state = STAT_PRE_COMMAND;
				}
				else
				{
					//state = STAT_COMMAND;
				}
				break;
			case STAT_PRE_COMMAND:
				if (c == 'x' || c == 'X')
				{
					state = STAT_COMMAND;
				}
				break;
			case STAT_COMMAND:
				if ((c != ',') && (c != ')'))
				{
					tmp[j++] = c;
				}
				else if (c == ',')
				{
					state = STAT_START_TAG;
				}
				else //����������
				{
					//�Խ����������������ݽ��д���
					//e.g. 0xFF800901
					tmp[j++] = '\0';
					printf("i=%d,%s\n", i, tmp);
					static int count = 0;
					fprintf(fp, "cmd%03d = <0x39>, <1>, <0>, <%d>, <%s>;\n", count++, data_format(tmp, fdata), fdata);// �����DTS��Ҫ��ĸ�ʽ
				}
				break;
			default:
				break;
			}
			n++;
		}

	}

	fclose(fp);
	return 0;
}

static void parser_format2()
{

	return;
}

static void parser_format3()
{

	return;
}
