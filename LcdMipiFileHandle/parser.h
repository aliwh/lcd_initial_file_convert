//
//解析LCD的模组厂提供的初始化文件，转换成对应的linux kernel DTS文件所需的格式（考虑以后转换成其他格式，增强功能）
//类似 cmd000 = <0x39>, <1>, <0>, <2>, <0xFF988107>;
//
#ifndef LCD_MIPI_PARSER_H
#define LCD_MIPI_PARSER_H

#define MAXSIZE 1000
#define MAXLEN 200

//保存输入输出的文件名
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


