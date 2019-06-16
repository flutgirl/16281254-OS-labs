#pragma once
#include "define.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void FileDescriptorToDisk(void);
void ChangeBitMap(int i, char p);
int SearchBitMap(void);
void WriteBlock(int i, char *p);
void ReadBlock(int i, char *p);
void InitDisk(void);
void InitBitMap(void);
void FileToDisk(char filename[]);
void DiskToFile(char filename[]);
void show_help();

//用于修改文件描述符
void ChangeFileDescriptor(FileDescriptor *f, int length, int DiskNum[], int Num, char p)
{
	f->Length = length;
	for (int i = 0; i<DiskNumLen; i++)

	{
		f->DiskNum[i] = DiskNum[i];
	}
	f->Num = Num;
	f->IsFree = p;
}
//初始化所有文件描述符
void InitFileDescriptor(void)
{
	for (int i = 0; i<256; i++)
	{
		int  DiskNum[3]; //磁盘号数组
		DiskNum[0] = i;
		DiskNum[1] = -1;
		DiskNum[2] = -1;
		int FileDescriptorNum = i; //文件描述符号
		ChangeFileDescriptor(&filedescriptor[i], 0, DiskNum, FileDescriptorNum, 'Y');
	}
	filedescriptor[0].IsFree = 'N';
	FileDescriptorToDisk();

}

//将文件描述符写入磁盘中
void FileDescriptorToDisk(void)
{
	char temp_block[512];
	int index = 0;
	int DiskNumIndex = 2;
	for (int i = 0; i<256; i++)
	{
		char temp_descriptor[24];
		memcpy(temp_descriptor, &filedescriptor[i], sizeof(FileDescriptor));
		memcpy(&temp_block[index * 24], temp_descriptor, 24);
		index++;
		int t = index % 21;
		if (t == 0)
		{
			index = 0;
			if (DiskNumIndex<10)
			{
				memcpy(ldisk[0][0][DiskNumIndex].Content, temp_block, 512);
				ChangeBitMap(DiskNumIndex, 'Y');
			}
			else
			{
				memcpy(ldisk[0][1][DiskNumIndex - B].Content, temp_block, 512);
				ChangeBitMap(DiskNumIndex, 'Y');//修改位图
			}
			DiskNumIndex++;
		}

	}
}

//将磁盘读取的信息恢复
void DiskToFileDescriptor(void)
{
	for (int i = 2; i<15; i++)
	{
		char temp[512];
		if (i<B)
			memcpy(temp, ldisk[0][0][i].Content, 512);
		else
			memcpy(temp, ldisk[0][1][i - B].Content, 512);
		for (int j = 0; j<21; j++)
		{
			if (((i - 2) * 21 + j)>256)
				break;
			char temp_FileDescriptor[24];
			memcpy(temp_FileDescriptor, &temp[j * 24], 24);
			FileDescriptor *f;
			f = (FileDescriptor*)temp_FileDescriptor;
			int num = (i - 2) * 21 + j;
			filedescriptor[num].IsFree = f->IsFree;
			filedescriptor[num].DiskNum[0] = f->DiskNum[0];
			filedescriptor[num].DiskNum[1] = f->DiskNum[1];
			filedescriptor[num].DiskNum[2] = f->DiskNum[2];
			filedescriptor[num].Length = f->Length;
			filedescriptor[num].Num = f->Num;
		}
	}
}

//初始化目录
void InitMenu(void)
{
	for (int i = 0; i<32; i++)
		menuitem[i].FileDescriptorNum = i;
}

//将目录内容写入文件描述中
void MenuToFileDescriptor(void)
{
	char temp_FileDescriptor[512];
	for (int i = 0; i<32; i++)
	{
		char temp_menuitem[16];
		memcpy(temp_menuitem, &menuitem[i], 16);
		memcpy(&temp_FileDescriptor[i * 16], temp_menuitem, 16);
	}
	filedescriptor[0].IsFree = 'N';
	filedescriptor[0].DiskNum[0] = SearchBitMap();
	filedescriptor[0].Length = 512;
	WriteBlock(filedescriptor[0].DiskNum[0], temp_FileDescriptor);
}

void FileDescriptorToMenu(void)
{
	char MenuContent[512];
	ReadBlock(filedescriptor[0].DiskNum[0], MenuContent);
	for (int i = 0; i<32; i++)
	{
		char temp_menuitem[16];
		memcpy(temp_menuitem, &MenuContent[i * 16], 16);
		MenuItem *t;
		t = (MenuItem *)temp_menuitem;
		strcpy(menuitem[i].FileName, t->FileName);
		menuitem[i].FileDescriptorNum = t->FileDescriptorNum;
	}
}

int SearchFileDescriptor()
{
	for (int i = 0; i<256; i++)
	{
		if (filedescriptor[i].IsFree == 'Y')
			return i;
	}
	return -1;
}

int SearchMenuItem()
{
	for (int i = 0; i<32; i++)
	{
		if (strlen(menuitem[i].FileName) == 0)
			return i;
	}
	return -1;
}

//下面是文件系统与用户直接的接口
void create(char filename[])
{
	int FileDescriptorNum, MenuItemNum, DiskNum;
	//寻找空闲目录项
	MenuItemNum = SearchMenuItem();
	strcpy(menuitem[MenuItemNum].FileName, filename);
	//寻找空闲文件描述符
	FileDescriptorNum = SearchFileDescriptor();
	menuitem[MenuItemNum].FileDescriptorNum = FileDescriptorNum;
	//寻找空闲磁盘块
	DiskNum = SearchBitMap();
	filedescriptor[FileDescriptorNum].DiskNum[0] = DiskNum;
	filedescriptor[FileDescriptorNum].IsFree = 'N';
	//修改磁盘位图
	ChangeBitMap(DiskNum, 'Y');
}

void destroy(char filename[])
{
	int MenuItemNum = -1;
	for (int i = 0; i<32; i++)
		if (strcmp(menuitem[i].FileName, filename) == 0)
			MenuItemNum = i;
	if (MenuItemNum == -1)
	{
		printf("目录中没有此文件！\n");
		return;
	}
	int FileDesCriptorNum = menuitem[MenuItemNum].FileDescriptorNum;
	//将目录项重置,重置时只需要将文件名删除，而不需要重置文件描述符，因为前面判断文件是否存在的条件是文件名是否存在
	memset(menuitem[MenuItemNum].FileName, 0, sizeof(menuitem[MenuItemNum].FileName));
	//修改文件描述符为空闲状态
	filedescriptor[FileDesCriptorNum].IsFree = 'Y';
	for (int i = 0; i<3; i++)
	{
		if (filedescriptor[FileDesCriptorNum].DiskNum[i] != -1)
			ChangeBitMap(filedescriptor[FileDesCriptorNum].DiskNum[i], 'N');
	}
}

int open(char filename[])
{
	int MenuItemNum = -1;
	for (int i = 0; i<32; i++)
		if (strcmp(menuitem[i].FileName, filename) == 0)
			MenuItemNum = i;
	if (MenuItemNum == -1)
	{
		printf("目录中没有此文件！\n");
		return -1;
	}
	else {
		//printf("该文件已经打开！\n");
		//返回文件描述符号
		return menuitem[MenuItemNum].FileDescriptorNum;
	}
}

char* read(int index, int mem_area, int count)
{
	char *temp;
	char block[512];
	temp = (char *)malloc(count * sizeof(char));
	ReadBlock(filedescriptor[index].DiskNum[0], block);
	memcpy(temp, &block[mem_area], count);
	return temp;
}

void write(int index, int mem_area, int count, char content[])
{
	char temp[512];
	char *s1 = (char*)malloc(mem_area * sizeof(char));
	char *s2 = (char*)malloc(mem_area * sizeof(char));
	char *s;
	int DiskNum = filedescriptor[index].DiskNum[0];
	ReadBlock(DiskNum, temp);
	memcpy(s1, temp, mem_area);
	memcpy(s2, &temp[mem_area], 512 - mem_area);
	s = strcat(s1, content);
	s = strcat(s, s2);
	filedescriptor[index].Length = strlen(s);
	WriteBlock(DiskNum, s);
}

char* ReadFile(char filename[])
{
	int index = open(filename);
	char content[512];
	ReadBlock(filedescriptor[index].DiskNum[0], content);
	printf("该文件内容如下：");
	printf("%s\n", content);
	return content;
}

void ChangeFileAdd(char filename[])
{
	int mem_area, count;
	char *s;
	int index = open(filename);
	char *p = ReadFile(filename);
	printf("请输入要写入的位置(向后写入）：");
	scanf("%d", &mem_area);
	printf("请输入写入的字节数：");
	scanf("%d", &count);
	s = (char*)malloc(count * sizeof(char));
	printf("请输入写入的内容:");
	scanf("%s", s);
	write(index, mem_area, count, s);
	return;
}

void ChangeFileDel(char filename[])
{
	int mem_area, count;
	int index = open(filename);
	printf("请输入要删除的位置(向后删除)：");
	scanf("%d", &mem_area);
	printf("请输入删除的字节数：");
	scanf("%d", &count);
	char temp[512];
	ReadBlock(filedescriptor[index].DiskNum[0], temp);
	char *s1 = (char*)malloc(mem_area * sizeof(char));
	char *s2 = (char*)malloc((512 - mem_area - count) * sizeof(char));
	memcpy(s1, temp, mem_area);
	memcpy(s2, &temp[mem_area + count], 512 - mem_area - count);
	s1 = strcat(s1, s2);
	WriteBlock(filedescriptor[index].DiskNum[0], s1);
	filedescriptor[index].Length = strlen(s1);
	ReadFile(filename);
	return;
}
void Init(void)
{
	InitDisk();
	InitBitMap();
	InitFileDescriptor();
	InitMenu();
}

void load(char filename[])
{
	FileToDisk(filename);
	DiskToFileDescriptor();
	FileDescriptorToMenu();
}

void save(char filename[])
{
	MenuToFileDescriptor();
	FileDescriptorToDisk();
	DiskToFile(filename);
}

int DiskNumToBlock(int c, int h, int b)
{
	return b + b * h + b * h*c;//返回
}

void InitDisk(void)
{
	for (int i = 0; i<C; i++)
		for (int j = 0; j<H; j++)
			for (int k = 0; k<B; k++)
			{
				ldisk[i][j][k].c = i;
				ldisk[i][j][k].h = j;
				ldisk[i][j][k].b = k;
				ldisk[i][j][k].BlockNnum = DiskNumToBlock(i, j, k);//计算对应的逻辑块号
			}
}

void ReadBlock(int i, char *p)
{
	int c, h, b;//磁盘的柱面 磁道 扇区
	c = i % (H*B);//
	h = (i - c * H*B) % B;//
	b = i - c * H*B - h * B;//
	memcpy(p, ldisk[c][h][b].Content, 512);
}

void WriteBlock(int i, char *p)
{
	int c, h, b;
	b = i % B;
	h = ((i - b) / B) % H;
	c = (i - b - h * B) / (H*B);
	b = i - c * H*B - h * B;
	memcpy(ldisk[c][h][b].Content, p, 512);
}
//将磁盘写入文件中
void DiskToFile(char filename[]) {
	//    FileDescriptorToDisk();
	FILE *fp;
	fp = fopen(filename, "wb");
	//判断fp打开成功
	if (fp == NULL)
	{
		printf("File Open Fail");
		exit(1);
	}
	// 循环遍历，将磁盘块内容写入二进制文件中去
	for (int i = 0; i<C; i++)
		for (int j = 0; j<H; j++)
			for (int k = 0; k<B; k++)
				//以二进制的形式写入二进制文件中
				fwrite(ldisk[i][j][k].Content, 512, 1, fp);
	fclose(fp);
}
//从文件中读取数据，恢复磁盘系统
void FileToDisk(char filename[]) {
	FILE *fp;
	fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		printf("File Open Fail");
		exit(1);
	}
	int index = 0;
	while (!feof(fp))
	{
		int c, h, b;
		b = index % B;
		h = ((index - b) / B) % H;
		c = (index - b - h * B) / (H*B);
		b = index - c * H*B - h * B;
		fread(ldisk[c][h][b].Content, 512, 1, fp);
		ldisk[c][h][b].c = c;
		ldisk[c][h][b].h = h;
		ldisk[c][h][b].b = b;
		index++;
	}
	fclose(fp);
}

//用于修改磁盘位图
void ChangeBitMap(int i, char p) {
	if (i <512)
		ldisk[0][0][0].Content[i] = p;
	else
		ldisk[0][0][1].Content[i - 512] = p;
}

//初始化磁盘位图
void InitBitMap(void) {
	//第0，1号磁盘已经被占用
	ChangeBitMap(0, 'Y');
	ChangeBitMap(1, 'Y');
	for (int i = 2; i<C*H*B; i++)
	{
		ChangeBitMap(i, 'N');
	}
}

//搜索位图，找到空闲磁盘块号
int SearchBitMap(void) {
	for (int i = 14; i<C*H*B; i++)
	{
		if (i<512)
		{
			if (ldisk[0][0][0].Content[i] == 'N')
				return i;
		}
		else
		{
			if (ldisk[0][0][1].Content[i - 512] == 'N')
				return i;
		}
	}
}

void show_help() {
	printf("**---------------------------------------------------**\n");
	printf("**         命令     说明         **\n");
	printf("**     1 ---- 显示目录内容    **\n");
	printf("**     2 ---- 新建一个文件    **\n");
	printf("**     3 ---- 删除一个文件    **\n");
	printf("**     4 ---- 打开指定文件    **\n");
	printf("**     5 ---- 读取指定文件    **\n");
	printf("**     6 ---- 写入到文件      **\n");
	printf("**     7 ---- 查看位图        **\n");
	printf("**     8 ---- 保存磁盘数据    **\n");
	printf("**     9 ---- 退出文件系统    **\n");
	printf("**---------------------------------------------------**\n");
}