#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define L 1000 //�߼�������
#define C 10 // �����
#define H 10 //��ͷ��
#define B 10 //������

#define DiskNumLen 3

//���̵ĵ�0��1���Ǵ洢λͼ
//���̵ĵ�2-9�����ڴ洢�ļ�������
//���������ļ��������ṹ��ռ��24�ֽ�

typedef struct BLOCK
{
	char Content[512]; //�߼���洢������
	int BlockNnum; //�߼����
	int c; // �����
	int h; //��ͷ��
	int b; //������
}BLOCK;

BLOCK ldisk[C][H][B];//����ģ��

typedef struct FileDescriptor //���ļ��������ܹ�ռ�ݴ���16�ֽ�
{
	int Length;//�ļ�����
	int DiskNum[DiskNumLen]; //�ڶ���3ֻ�Ǳ�ʾÿ�����̿����󳤶���3λ
	int Num; //�ļ���������
	char IsFree; //��ʾ���ļ��������Ƿ����
}FileDescriptor;

FileDescriptor filedescriptor[256];

//Ŀ¼��Ӧ0���ļ�������,һ��Ŀ¼��ռ��16�ֽڣ�����һ���ļ����������Դ洢96���ļ�
typedef struct contents 
{
	char FileName[12]; //Ŀ¼�����ļ�������󳤶�Ϊ16�ֽ�
	int FileDescriptorNum;//�ļ���������
}contents;
contents menuitem[31];//Ŀ¼������

					 