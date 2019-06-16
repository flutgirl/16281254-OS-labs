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
	printf("�ļ�ϵͳ׼�����...\n");
	while (1)
	{
		int choice2;
		int flag = 0;
		printf_s("��ӭʹ���ļ�ϵͳ��ʹ�ð������£�\n");
		show_help();
		printf("����������������ţ�");
		scanf("%d", &choice2);
		switch (choice2)
		{
		case 1:ShowDir(); break;
		case 2:
			printf("������Ҫ�������ļ�����");
			scanf("%s", filename);
			create(filename);
			printf("���ļ��Ѿ�������\n");
			break;
		case 3:
			printf("������Ҫɾ�����ļ�����");
			scanf("%s", filename);
			destroy(filename);
			break;
		case 4:
			printf("������Ҫ�򿪵��ļ�����");
			scanf("%s", filename);
			open(filename);
			printf("���ļ��Ѿ��򿪣�\n");
			break;
		case 5:
			printf("������Ҫ��ȡ���ļ�����");
			scanf("%s", filename);
			ReadFile(filename);
			break;
		case 6:
			
			printf("������Ҫд����ļ���:");
			scanf("%s", filename);
				ChangeFileAdd(filename);

			break;
		case 7:
			ShowBitMap();
			break;
		case 8:
			printf("������Ҫ������ļ�����");
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
	printf("----------------�ļ�Ŀ¼--------------\n");
	printf("������\t\t��С \t\t �ļ���\n ");
	for (int i = 0; i<32; i++)
	{
		if (strlen(menuitem[i].FileName) != 0)
		{
			printf("%d\t\t %s\t\t %dB\n", index, menuitem[i].FileName, filedescriptor[menuitem[i].FileDescriptorNum].Length);
			exist++;
		}
		index++;
	}
	printf("��ǰĿ¼�¹���%d���ļ�\n", exist);
	printf("--------------------------------------\n");
}

void ShowBitMap(void)
{
	printf("\n****************λͼ**********************\n");
	int used = 0;
	printf("��ǰ�Ĵ���ʹ���������(Y��ʾʹ�ã�N��ʾδʹ��)\n");
	for (int i = 0; i<C; i++)
	{
		printf("%d�����������Ϣ����:\n", i);
		printf("   ��:0 1 2 3 4 5 6 7 8 9\n");
		printf("ͷ\n");
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
	printf("�ܹ�ʹ��%d�����̿飬ʣ��%d�����̿����\n", used, (C*B*H - used));
	printf("****************************************\n");
}
