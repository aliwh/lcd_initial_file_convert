// LcdMipiFileHandle.cpp : 定义控制台应用程序的入口点。
//

//TODO:需要改进的地方
//1.只能处理H:\\ILI9881_CPT5.5_MIPI.txt这一个文件
//2.只能输出一个文件名,最好在输出的文件名上增加时间，用于区分
//3.处理的LCD命令文件格式需要更多元，更通用(已修改了程序架构，单独抽象出了解析器)
//4.程度的架构需要优化下、编程规范化
//5.程序本身存在一些bug需要fix（a.最后一个命令打印不出来的问题（Done:已解）;b.命令的长度问题）
//6.代码通过Git进行管理（Done:已挂靠到github服务器上）
//7.函数内数组的大小，因分配在堆栈中，不能分太大，可以考虑放到函数外或加个static关键字(不需要了，函数中不用分配那么大的数组)

#include "stdafx.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "parser.h" //lcd init 文件解析器

int _tmain(int argc, _TCHAR* argv[])
{
	int n = 0;

	int i = 0, j = 0, row = 0;;
	int ret;
	struct path file_path; //从标准输入中读取文件路径

	printf("Please input file path：");
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

	//若解析成功，且转换成功，将转换后的文件名及路径告知用户
	printf("Congratulation! \nfinal file store in <.\\LcdMipiFileHandle\\mipifiles >\n");
	
	return 0;

}





