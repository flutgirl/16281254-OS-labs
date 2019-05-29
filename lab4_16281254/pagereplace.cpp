#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct item//ҳ��
{
	int page;		//ҳ��
	int wtime;		//�ȴ�ʱ�䣬LRU�㷨���õ��������
	int fwen;          //����λ��Clock�㷨���õ��������
	int xgai;          //�޸�λ��Clock�㷨���õ��������
} Pro;
Pro *page;			//��ҵҳ�漯
Pro *memory;		//�ڴ�ҳ�漯

int pageSeq[10000];   //ҳ���������
int pageNum;		//ϵͳ�������ҵ�������е�ҳ����
int memoryNum;		//�����ڴ�ҳ����
int curmemory;		//�����ڴ��е�ҳ�����
int miss_num;		//ȱҳ����
float miss_rate;		//ȱҳ��



void print(Pro *memory1)//��ӡ��ǰ��ҳ��
{
	int j;
	for (j = 0; j<memoryNum; j++)
		printf("%d ", memory1[j].page);
	printf("\n");
}

int Search(int num1, Pro *memory1)//��ҳ�漯memory1�в���num1������ҵ�����������memory1�е��±꣬���򷵻�-1
{
	int j;
	for (j = 0; j<memoryNum; j++)
	{
		if (num1 == memory1[j].page)
		{
			memory[j].xgai = 1;
			return j;
		}
	}
	return -1;
}

int SequenceCreation()//����ҳ���������
{
	int N, P, e, m, i = 0, j = 0, k = 0, judge = 0;
	float t, r;
	printf("�����������ڴ��С(3=<N<=64)��\n");
	scanf_s("%d", &N);
	while (N>64)
	{
		printf("�����ڴ��������ҳ��Ϊ64,���������������ڴ��С��\n");
		scanf_s("%d", &N);
	}
	printf("�����빤��������ʼλ��(P<32)��\n");
	scanf_s("%d", &P);
	printf("�����빤����������ҳ��(e<32)��\n");
	scanf_s("%d", &e);
	printf("�����빤�����ƶ���(m<32)��\n");
	scanf_s("%d", &m);
	srand((unsigned)time(NULL));
	t = 0.5;
	do
	{
		for (j = 0; j<m; j++)
		{
			pageSeq[j + k] = ((rand() % e) + P) % N;
		}
		k += m;
		r = rand() / 32767.0;
		if (r < t)
		{
			P = rand() % N;
		}
		else
		{
			P = (P + 1) % N;
		}
		printf("���������Ӵ�ҳ��������д��ĳ���,�����룺1����������0��\n");
		printf("��ʱ�������д��ĳ���Ϊ��%d\n", k);
		scanf_s("%d", &judge);
	} while (judge == 1);
	printf("ҳ��������д�Ϊ��\n");
	for (i = 0; i < k; i++)
	{
		printf("%d ", pageSeq[i]);
	}
	printf("\n");
	return k;//����ҳ��������д��ĳ���
}

int FIFO()//�Ƚ��ȳ�ҳ���û��㷨
{
	int i;
	miss_num = 0;
	printf("�Ƚ��ȳ�ҳ���û��㷨ҳ���û����:   \n");
	for (i = 0; i<pageNum; i++)
	{
		if (Search(page[i].page, memory)<0)//�����ڴ���û���ҵ���ҳ��
		{
			miss_num++;
			memory[curmemory].page = page[i].page;
			print(memory);
			curmemory = (curmemory + 1) % memoryNum;   //�ҳ����Ƚ����ڴ��ҳ��
		}
	}
	miss_rate = (float)miss_num / pageNum;
	printf("ȱҳ������%d   ȱҳ��:  %f\n\n", miss_num, miss_rate);
	return 0;
}

int OPT()//����û��㷨
{
	int i;
	miss_num = 0;
	curmemory = 0;
	printf("����û��㷨ҳ���û����:   \n");
	for (i = 0; i<pageNum; i++)
	{
		if (Search(page[i].page, memory) < 0)//�����ڴ���û���ҵ���ҳ��
		{
			//�ҳ�δ���ʱ���ڲ��ٱ����ʵ�ҳ��
			int tem;
			int opt = 0;
			int k;
			for (k = 0; k < memoryNum; k++)
			{
				if (memory[k].page == -1)
				{
					curmemory = k;
					break;
				}
				tem = 0;       //ҳ��k��δ��temʱ���ڲ������
				int j;
				for (j = i + 1; j < pageNum; j++)
				{
					if (page[j].page == memory[k].page)
					{
						if (tem > opt)
						{
							opt = tem;
							curmemory = k;
						}
						break;
					}
					else tem++;
				}
				if (j == pageNum)
				{
					opt = tem;
					curmemory = k;
				}
			}
			miss_num++;
			memory[curmemory].page = page[i].page;
			print(memory);
		}
	}
	miss_rate = (float)miss_num / pageNum;
	printf("ȱҳ������%d   ȱҳ��:  %f\n\n", miss_num, miss_rate);
	return 0;
}

int LRU()//������δʹ���û��㷨
{
	int i;
	miss_num = 0;
	curmemory = 0;
	printf("������δʹ���û��㷨ҳ���û����:   \n");
	for (i = 0; i<pageNum; i++)
	{
		int rec = Search(page[i].page, memory);
		if (rec < 0)    //�����ڴ���û���ҵ���ҳ��
		{
			miss_num++;
			int j;
			for (j = 0; j<memoryNum; j++)     //�ҳ�������δʹ�õ�ҳ��
				if (memory[j].wtime == -1)
				{
					curmemory = j;
					break;
				}
				else if (memory[j].wtime > memory[curmemory].wtime)
					curmemory = j;

			memory[curmemory].page = page[i].page;
			memory[curmemory].wtime = 0;
			print(memory);

		}
		else memory[rec].wtime = 0;
		int j;
		for (j = 0; j<memoryNum; j++)     //�ڴ��е�����ҳ��ȴ�ʱ��+1
			if (memory[j].page != -1)
				memory[j].wtime++;

	}//end for
	miss_rate = (float)miss_num / pageNum;
	printf("ȱҳ������%d ȱҳ��: %f\n\n", miss_num, miss_rate);
	return 0;
}

int NewClock()//�Ľ���Clock�û��㷨
{
	int i;
	int seek = 0;
	miss_num = 0;
	curmemory = 0;
	printf("�Ľ���Clock�û��㷨ҳ���û����:   \n");

	for (i = 0; i<pageNum; i++)
	{
		seek = 0;
		int rec = Search(page[i].page, memory);
		if (rec < 0)    //�����ڴ���û���ҵ���ҳ��
		{
			int j;
			miss_num++;
			for (j = 0; j<memoryNum; j++)		//���ѡ���޸�λ
			{
				memory[j].xgai = rand() % 2;
			}
			do
			{
				for (j = 0; j<memoryNum; j++)     //Ѱ���Ƿ��п��������
				{
					if (memory[j].page == -1)
					{
						curmemory = j;
						seek = 1;
						memory[j].page = 0;
						break;
					}

				}
				if (seek != 1)
				{
					for (j = 0; j<memoryNum; j++)     //Ѱ�������δ�����ʣ���Ϊ���޸ĵ�ҳ��
					{
						if (memory[j].fwen == 0 && memory[j].xgai == 0)
						{
							curmemory = j;
							seek = 1;
							break;
						}
					}
					if (seek != 1)
					{
						for (j = 0; j<memoryNum; j++)     //Ѱ�������δ�����ʣ����ѱ��޸ĵ�ҳ��
						{
							if (memory[j].fwen == 0 && memory[j].xgai == 1)
							{
								curmemory = j;
								seek = 1;
								break;
							}
							memory[j].fwen = 0;
						}
					}
				}
			} while (seek == 0);
		}
		memory[curmemory].page = page[i].page;
		print(memory);
	}
	miss_rate = (float)miss_num / pageNum;
	printf("ȱҳ������%d ȱҳ��: %f\n\n", miss_num, miss_rate);
	return 0;
}

int PBA()//ҳ�滺���㷨(PBA)
{
	int i, j, k;
	int length = 3;//����ҳ������
	miss_num = 0;
	Pro *memory2;		//����ҳ������
	memory2 = (Pro*)malloc(sizeof(Pro)*length);
	for (i = 0; i<length; i++)
	{
		page[i].page = -1;
		page[i].wtime = -1;
		page[i].fwen = 0;
		page[i].xgai = 0;
	}
	printf("ҳ�滺���㷨ҳ���û����:   \n");

	for (i = 0; i<pageNum; i++)
	{
		if (Search(page[i].page, memory)<0 && Search(page[i].page, memory2)<0)//�����ڴ漰����������û���ҵ���ҳ��
		{
			miss_num++;
			if (memory[curmemory].page == -1)
			{
				memory[curmemory].page = page[i].page;
				print(memory);
				curmemory = (curmemory + 1) % memoryNum;   //�ҳ����Ƚ����ڴ��ҳ��
			}
			else
			{
				if (memory[curmemory].xgai == 0)
				{
					for (j = 0; j<length; j++)
					{
						if (memory2[j].page == -1)
						{
							memory2[j].page = memory[curmemory].page;
							memory2[j].xgai = memory[curmemory].xgai;
							break;
						}
					}
					memory[curmemory].page = page[i].page;
					print(memory);
					curmemory = (curmemory + 1) % memoryNum;   //�ҳ����Ƚ����ڴ��ҳ��
				}
				else
				{
					memory[curmemory].page = page[i].page;
					print(memory);
					curmemory = (curmemory + 1) % memoryNum;
				}
			}
		}
		else
		{
			if (Search(page[i].page, memory2) >= 0)
			{
				for (j = 0; j<length; j++)
				{
					if (memory2[j].page == page[i].page)
					{
						for (k = j; k>0; k--)
						{
							memory2[k].page = memory2[k - 1].page;
							memory2[k].xgai = memory2[k - 1].xgai;
						}
						memory2[0].page = memory[curmemory].page;
					}
				}
			}
		}
	}
	miss_rate = (float)miss_num / pageNum;
	printf("ȱҳ������%d ȱҳ��: %f\n\n", miss_num, miss_rate);
	free(memory2);
	return 0;
}



int main(void)//������
{
	int i;
	char c;		//�õ��û��������ַ�����ѡ����Ӧ���û��㷨
	pageNum = SequenceCreation();   //����ҳ���������
	printf("����ϵͳ�����ڴ�ҳ����:\n");
	scanf_s("%d", &memoryNum);
	page = (Pro*)malloc(sizeof(Pro)*pageNum);
	memory = (Pro*)malloc(sizeof(Pro)*memoryNum);

	for (i = 0; i<pageNum; i++)
	{
		page[i].page = pageSeq[i];
		page[i].wtime = 0;
		page[i].fwen = 0;
		page[i].xgai = 0;
	}
	do
	{
		for (i = 0; i<memoryNum; i++)		//��ʼ���ڴ���ҳ��
		{
			memory[i].page = -1;				//ҳ��Ϊ����-1��ʾ
			memory[i].wtime = -1;
			memory[i].fwen = 0;
			memory[i].xgai = 0;
		}
		printf("�������ʾѡ������ҳ���û��㷨��\n");
		printf("     1:��ѣ�Optimal)�û��㷨\n");
		printf("     2:�Ƚ��ȳ�(FIFO)�û��㷨\n");
		printf("     3:������δʹ��(Least Recently Used)�û��㷨\n");
		printf("     4:�Ľ���Clock�û��㷨\n");
		printf("     5:ҳ�滺���㷨(PBA)\n");
		printf("*****��ѡ���������(1,2,3,4,5),������������******\n");
		getchar();
		scanf_s("%c", &c);
		curmemory = 0;
		if (c == '1')			//��ѣ�Optimal)�û��㷨
		{
			OPT();
		}
		if (c == '2')           //�Ƚ��ȳ�(FIFO)�û��㷨
		{
			FIFO();
		}
		if (c == '3')			//������δʹ��(Least Recently Used)�û��㷨
		{
			LRU();
		}
		if (c == '4')			//�Ľ���Clock�û��㷨
		{
			NewClock();
		}
		if (c == '5')			//ҳ�滺���㷨(PBA)
		{
			PBA();
		}
	} while (c == '1' || c == '2' || c == '3' || c == '4' || c == '5');
	free(page);
	free(memory);
	return 0;
}