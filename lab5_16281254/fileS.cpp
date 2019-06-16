#include <stdio.h>
#include "define.h"
#include "operation.h"
#include <string.h>

void ShowDir();
void ShowBitMap(void);

int main()
{
	int choice;
	char filename[12];
	Init();
	printf("文件系统准备完毕...\n");
	while (1)
	{
		int choice2;
		int flag = 0;
		printf_s("欢迎使用文件系统，使用帮助如下：\n");
		show_help();
		printf("现在请输入命令序号：");
		scanf("%d", &choice2);
		switch (choice2)
		{
		case 1:ShowDir(); break;
		case 2:
			printf("请输入要创建的文件名：");
			scanf("%s", filename);
			create(filename);
			printf("该文件已经创建！\n");
			break;
		case 3:
			printf("请输入要删除的文件名：");
			scanf("%s", filename);
			destroy(filename);
			break;
		case 4:
			printf("请输入要打开的文件名：");
			scanf("%s", filename);
			open(filename);
			printf("该文件已经打开！\n");
			break;
		case 5:
			printf("请输入要读取的文件名：");
			scanf("%s", filename);
			ReadFile(filename);
			break;
		case 6:
			
			printf("请输入要写入的文件名:");
			scanf("%s", filename);
				ChangeFileAdd(filename);

			break;
		case 7:
			ShowBitMap();
			break;
		case 8:
			printf("请输入要保存的文件名：");
			scanf("%s", filename);
			save(filename);
			break;
		case 9:
			flag = 1;
			break;
		default:
			break;
		}
		if (flag)
			break;

	}

	return 0;
}

void ShowDir()
{
	int index = 1;
	int exist = 0;
	printf("----------------文件目录--------------\n");
	printf("索引号\t\t大小 \t\t 文件名\n ");
	for (int i = 0; i<32; i++)
	{
		if (strlen(menuitem[i].FileName) != 0)
		{
			printf("%d\t\t %s\t\t %dB\n", index, menuitem[i].FileName, filedescriptor[menuitem[i].FileDescriptorNum].Length);
			exist++;
		}
		index++;
	}
	printf("当前目录下共有%d个文件\n", exist);
	printf("--------------------------------------\n");
}

void ShowBitMap(void)
{
	printf("\n****************位图**********************\n");
	int used = 0;
	printf("当前的磁盘使用情况如下(Y表示使用，N表示未使用)\n");
	for (int i = 0; i<C; i++)
	{
		printf("%d号柱面磁盘信息如下:\n", i);
		printf("   区:0 1 2 3 4 5 6 7 8 9\n");
		printf("头\n");
		for (int j = 0; j<H; j++)
		{
			printf("%d\t:", j);
			for (int k = 0; k<B; k++)
			{
				int t = i * H*B + j * B + k;
				if (t<512)
				{
					printf("%c ", ldisk[0][0][0].Content[t]);
					if (ldisk[0][0][0].Content[t] == 'Y')
						used++;
				}
				else
				{
					printf("%c ", ldisk[0][0][1].Content[t - 512]);
					if (ldisk[0][0][0].Content[t] == 'Y')
						used++;
				}
			}
			printf("\n");
		}
	}
	printf("总共使用%d个磁盘块，剩余%d个磁盘块空闲\n", used, (C*B*H - used));
	printf("****************************************\n");
}
