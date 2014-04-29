//
//����LCD��ģ�鳧�ṩ�ĳ�ʼ���ļ���ת���ɶ�Ӧ��linux kernel DTS�ļ�����ĸ�ʽ�������Ժ�ת����������ʽ����ǿ���ܣ�
//���� cmd000 = <0x39>, <1>, <0>, <2>, <0xFF988107>;
//
#ifndef LCD_MIPI_PARSER_H
#define LCD_MIPI_PARSER_H

#define MAXSIZE 1000
#define MAXLEN 200

//��������������ļ���
struct path{
	char input[100];
	char output[100];
};

enum Format
{
	CONVERT_FORMAT0 = 0,
	CONVERT_FORMAT1,
	CONVERT_FORMAT2,
	CONVERT_FORMAT3,
	CONVERT_FORMAT4,
	CONVERT_FORMAT5,
	CONVERT_FORMAT6
};

int parser_mipi_file(struct path* path, int parser_format);

#endif


