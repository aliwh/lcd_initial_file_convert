// LcdMipiFileHandle.cpp : �������̨Ӧ�ó������ڵ㡣
//

//TODO:��Ҫ�Ľ��ĵط�
//1.ֻ�ܴ���H:\\ILI9881_CPT5.5_MIPI.txt��һ���ļ�
//2.ֻ�����һ���ļ���,�����������ļ���������ʱ�䣬��������
//3.�����LCD�����ļ���ʽ��Ҫ����Ԫ����ͨ��(���޸��˳���ܹ�������������˽�����)
//4.�̶ȵļܹ���Ҫ�Ż��¡���̹淶��
//5.���������һЩbug��Ҫfix��a.���һ�������ӡ�����������⣨Done:�ѽ⣩;b.����ĳ������⣩
//6.����ͨ��Git���й���Done:�ѹҿ���github�������ϣ�
//7.����������Ĵ�С��������ڶ�ջ�У����ܷ�̫�󣬿��Կ��Ƿŵ��������Ӹ�static�ؼ���(����Ҫ�ˣ������в��÷�����ô�������)

#include "stdafx.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "parser.h" //lcd init �ļ�������

int _tmain(int argc, _TCHAR* argv[])
{
	int n = 0;

	int i = 0, j = 0, row = 0;;
	int ret;
	struct path file_path; //�ӱ�׼�����ж�ȡ�ļ�·��

	printf("Please input file path��");
	ret = scanf("%s", file_path.input);
	if (ret != 1)
	{
		printf("Invalid input!\n");
		return -1;
	}

	ret = parser_mipi_file(&file_path, CONVERT_FORMAT0);
	if (ret < 0)
	{
		printf("Convert error\n");
		return -1;
	}

	//�������ɹ�����ת���ɹ�����ת������ļ�����·����֪�û�
	printf("Congratulation! \nfinal file store in <.\\LcdMipiFileHandle\\mipifiles >\n");
	
	return 0;

}





