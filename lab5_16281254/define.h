#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define L 1000 //逻辑块数量
#define C 10 // 柱面号
#define H 10 //磁头号
#define B 10 //扇区号

#define DiskNumLen 3

//磁盘的第0、1块是存储位图
//磁盘的第2-9块用于存储文件描述符
//对齐后这个文件描述符结构体占据24字节

typedef struct BLOCK
{
	char Content[512]; //逻辑块存储的内容
	int BlockNnum; //逻辑块号
	int c; // 柱面号
	int h; //磁头号
	int b; //扇区号
}BLOCK;

BLOCK ldisk[C][H][B];//磁盘模型

typedef struct FileDescriptor //此文件描述符总共占据磁盘16字节
{
	int Length;//文件长度
	int DiskNum[DiskNumLen]; //第二个3只是表示每个磁盘块号最大长度是3位
	int Num; //文件描述符号
	char IsFree; //表示此文件描述符是否空闲
}FileDescriptor;

FileDescriptor filedescriptor[256];

//目录对应0号文件描述符,一个目录项占据16字节，所以一个文件描述符可以存储96个文件
typedef struct contents 
{
	char FileName[12]; //目录项中文件名的最大长度为16字节
	int FileDescriptorNum;//文件描述符号
}contents;
contents menuitem[31];//目录项数组

					 