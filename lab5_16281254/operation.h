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

//�����޸��ļ�������
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
//��ʼ�������ļ�������
void InitFileDescriptor(void)
{
	for (int i = 0; i<256; i++)
	{
		int  DiskNum[3]; //���̺�����
		DiskNum[0] = i;
		DiskNum[1] = -1;
		DiskNum[2] = -1;
		int FileDescriptorNum = i; //�ļ���������
		ChangeFileDescriptor(&filedescriptor[i], 0, DiskNum, FileDescriptorNum, 'Y');
	}
	filedescriptor[0].IsFree = 'N';
	FileDescriptorToDisk();

}

//���ļ�������д�������
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
				ChangeBitMap(DiskNumIndex, 'Y');//�޸�λͼ
			}
			DiskNumIndex++;
		}

	}
}

//�����̶�ȡ����Ϣ�ָ�
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

//��ʼ��Ŀ¼
void InitMenu(void)
{
	for (int i = 0; i<32; i++)
		menuitem[i].FileDescriptorNum = i;
}

//��Ŀ¼����д���ļ�������
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

//�������ļ�ϵͳ���û�ֱ�ӵĽӿ�
void create(char filename[])
{
	int FileDescriptorNum, MenuItemNum, DiskNum;
	//Ѱ�ҿ���Ŀ¼��
	MenuItemNum = SearchMenuItem();
	strcpy(menuitem[MenuItemNum].FileName, filename);
	//Ѱ�ҿ����ļ�������
	FileDescriptorNum = SearchFileDescriptor();
	menuitem[MenuItemNum].FileDescriptorNum = FileDescriptorNum;
	//Ѱ�ҿ��д��̿�
	DiskNum = SearchBitMap();
	filedescriptor[FileDescriptorNum].DiskNum[0] = DiskNum;
	filedescriptor[FileDescriptorNum].IsFree = 'N';
	//�޸Ĵ���λͼ
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
		printf("Ŀ¼��û�д��ļ���\n");
		return;
	}
	int FileDesCriptorNum = menuitem[MenuItemNum].FileDescriptorNum;
	//��Ŀ¼������,����ʱֻ��Ҫ���ļ���ɾ����������Ҫ�����ļ�����������Ϊǰ���ж��ļ��Ƿ���ڵ��������ļ����Ƿ����
	memset(menuitem[MenuItemNum].FileName, 0, sizeof(menuitem[MenuItemNum].FileName));
	//�޸��ļ�������Ϊ����״̬
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
		printf("Ŀ¼��û�д��ļ���\n");
		return -1;
	}
	else {
		//printf("���ļ��Ѿ��򿪣�\n");
		//�����ļ���������
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
	printf("���ļ��������£�");
	printf("%s\n", content);
	return content;
}

void ChangeFileAdd(char filename[])
{
	int mem_area, count;
	char *s;
	int index = open(filename);
	char *p = ReadFile(filename);
	printf("������Ҫд���λ��(���д�룩��");
	scanf("%d", &mem_area);
	printf("������д����ֽ�����");
	scanf("%d", &count);
	s = (char*)malloc(count * sizeof(char));
	printf("������д�������:");
	scanf("%s", s);
	write(index, mem_area, count, s);
	return;
}

void ChangeFileDel(char filename[])
{
	int mem_area, count;
	int index = open(filename);
	printf("������Ҫɾ����λ��(���ɾ��)��");
	scanf("%d", &mem_area);
	printf("������ɾ�����ֽ�����");
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
	return b + b * h + b * h*c;//����
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
				ldisk[i][j][k].BlockNnum = DiskNumToBlock(i, j, k);//�����Ӧ���߼����
			}
}

void ReadBlock(int i, char *p)
{
	int c, h, b;//���̵����� �ŵ� ����
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
//������д���ļ���
void DiskToFile(char filename[]) {
	//    FileDescriptorToDisk();
	FILE *fp;
	fp = fopen(filename, "wb");
	//�ж�fp�򿪳ɹ�
	if (fp == NULL)
	{
		printf("File Open Fail");
		exit(1);
	}
	// ѭ�������������̿�����д��������ļ���ȥ
	for (int i = 0; i<C; i++)
		for (int j = 0; j<H; j++)
			for (int k = 0; k<B; k++)
				//�Զ����Ƶ���ʽд��������ļ���
				fwrite(ldisk[i][j][k].Content, 512, 1, fp);
	fclose(fp);
}
//���ļ��ж�ȡ���ݣ��ָ�����ϵͳ
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

//�����޸Ĵ���λͼ
void ChangeBitMap(int i, char p) {
	if (i <512)
		ldisk[0][0][0].Content[i] = p;
	else
		ldisk[0][0][1].Content[i - 512] = p;
}

//��ʼ������λͼ
void InitBitMap(void) {
	//��0��1�Ŵ����Ѿ���ռ��
	ChangeBitMap(0, 'Y');
	ChangeBitMap(1, 'Y');
	for (int i = 2; i<C*H*B; i++)
	{
		ChangeBitMap(i, 'N');
	}
}

//����λͼ���ҵ����д��̿��
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
	printf("**         ����     ˵��         **\n");
	printf("**     1 ---- ��ʾĿ¼����    **\n");
	printf("**     2 ---- �½�һ���ļ�    **\n");
	printf("**     3 ---- ɾ��һ���ļ�    **\n");
	printf("**     4 ---- ��ָ���ļ�    **\n");
	printf("**     5 ---- ��ȡָ���ļ�    **\n");
	printf("**     6 ---- д�뵽�ļ�      **\n");
	printf("**     7 ---- �鿴λͼ        **\n");
	printf("**     8 ---- �����������    **\n");
	printf("**     9 ---- �˳��ļ�ϵͳ    **\n");
	printf("**---------------------------------------------------**\n");
}