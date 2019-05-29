#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct item//页表
{
	int page;		//页号
	int wtime;		//等待时间，LRU算法会用到这个属性
	int fwen;          //访问位，Clock算法会用到这个属性
	int xgai;          //修改位，Clock算法会用到这个属性
} Pro;
Pro *page;			//作业页面集
Pro *memory;		//内存页面集

int pageSeq[10000];   //页面访问序列
int pageNum;		//系统分配给作业的主存中的页面数
int memoryNum;		//可用内存页面数
int curmemory;		//调入内存中的页面个数
int miss_num;		//缺页次数
float miss_rate;		//缺页率



void print(Pro *memory1)//打印当前的页面
{
	int j;
	for (j = 0; j<memoryNum; j++)
		printf("%d ", memory1[j].page);
	printf("\n");
}

int Search(int num1, Pro *memory1)//在页面集memory1中查找num1，如果找到，返回其在memory1中的下标，否则返回-1
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

int SequenceCreation()//创建页面访问序列
{
	int N, P, e, m, i = 0, j = 0, k = 0, judge = 0;
	float t, r;
	printf("请输入虚拟内存大小(3=<N<=64)：\n");
	scanf_s("%d", &N);
	while (N>64)
	{
		printf("虚拟内存限制最大页数为64,请重新输入虚拟内存大小：\n");
		scanf_s("%d", &N);
	}
	printf("请输入工作集的起始位置(P<32)：\n");
	scanf_s("%d", &P);
	printf("请输入工作集包含的页数(e<32)：\n");
	scanf_s("%d", &e);
	printf("请输入工作集移动率(m<32)：\n");
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
		printf("如果想继续加大页面访问序列串的长度,请输入：1，否则输入0；\n");
		printf("此时访问序列串的长度为：%d\n", k);
		scanf_s("%d", &judge);
	} while (judge == 1);
	printf("页面访问序列串为：\n");
	for (i = 0; i < k; i++)
	{
		printf("%d ", pageSeq[i]);
	}
	printf("\n");
	return k;//返回页面访问序列串的长度
}

int FIFO()//先进先出页面置换算法
{
	int i;
	miss_num = 0;
	printf("先进先出页面置换算法页面置换情况:   \n");
	for (i = 0; i<pageNum; i++)
	{
		if (Search(page[i].page, memory)<0)//若在内存中没有找到该页面
		{
			miss_num++;
			memory[curmemory].page = page[i].page;
			print(memory);
			curmemory = (curmemory + 1) % memoryNum;   //找出最先进入内存的页面
		}
	}
	miss_rate = (float)miss_num / pageNum;
	printf("缺页次数：%d   缺页率:  %f\n\n", miss_num, miss_rate);
	return 0;
}

int OPT()//最佳置换算法
{
	int i;
	miss_num = 0;
	curmemory = 0;
	printf("最佳置换算法页面置换情况:   \n");
	for (i = 0; i<pageNum; i++)
	{
		if (Search(page[i].page, memory) < 0)//若在内存中没有找到该页面
		{
			//找出未来最长时间内不再被访问的页面
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
				tem = 0;       //页面k在未来tem时间内不会出现
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
	printf("缺页次数：%d   缺页率:  %f\n\n", miss_num, miss_rate);
	return 0;
}

int LRU()//最近最久未使用置换算法
{
	int i;
	miss_num = 0;
	curmemory = 0;
	printf("最近最久未使用置换算法页面置换情况:   \n");
	for (i = 0; i<pageNum; i++)
	{
		int rec = Search(page[i].page, memory);
		if (rec < 0)    //若在内存中没有找到该页面
		{
			miss_num++;
			int j;
			for (j = 0; j<memoryNum; j++)     //找出最近最久未使用的页面
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
		for (j = 0; j<memoryNum; j++)     //内存中的所有页面等待时间+1
			if (memory[j].page != -1)
				memory[j].wtime++;

	}//end for
	miss_rate = (float)miss_num / pageNum;
	printf("缺页次数：%d 缺页率: %f\n\n", miss_num, miss_rate);
	return 0;
}

int NewClock()//改进型Clock置换算法
{
	int i;
	int seek = 0;
	miss_num = 0;
	curmemory = 0;
	printf("改进型Clock置换算法页面置换情况:   \n");

	for (i = 0; i<pageNum; i++)
	{
		seek = 0;
		int rec = Search(page[i].page, memory);
		if (rec < 0)    //若在内存中没有找到该页面
		{
			int j;
			miss_num++;
			for (j = 0; j<memoryNum; j++)		//随机选择修改位
			{
				memory[j].xgai = rand() % 2;
			}
			do
			{
				for (j = 0; j<memoryNum; j++)     //寻找是否有空闲物理块
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
					for (j = 0; j<memoryNum; j++)     //寻找最近即未被访问，又为被修改的页面
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
						for (j = 0; j<memoryNum; j++)     //寻找最近即未被访问，但已被修改的页面
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
	printf("缺页次数：%d 缺页率: %f\n\n", miss_num, miss_rate);
	return 0;
}

int PBA()//页面缓冲算法(PBA)
{
	int i, j, k;
	int length = 3;//空闲页面链表
	miss_num = 0;
	Pro *memory2;		//空闲页面链表
	memory2 = (Pro*)malloc(sizeof(Pro)*length);
	for (i = 0; i<length; i++)
	{
		page[i].page = -1;
		page[i].wtime = -1;
		page[i].fwen = 0;
		page[i].xgai = 0;
	}
	printf("页面缓冲算法页面置换情况:   \n");

	for (i = 0; i<pageNum; i++)
	{
		if (Search(page[i].page, memory)<0 && Search(page[i].page, memory2)<0)//若在内存及空闲链表中没有找到该页面
		{
			miss_num++;
			if (memory[curmemory].page == -1)
			{
				memory[curmemory].page = page[i].page;
				print(memory);
				curmemory = (curmemory + 1) % memoryNum;   //找出最先进入内存的页面
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
					curmemory = (curmemory + 1) % memoryNum;   //找出最先进入内存的页面
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
	printf("缺页次数：%d 缺页率: %f\n\n", miss_num, miss_rate);
	free(memory2);
	return 0;
}



int main(void)//主函数
{
	int i;
	char c;		//得到用户的输入字符，来选择相应的置换算法
	pageNum = SequenceCreation();   //创建页面访问序列
	printf("输入系统分配内存页面数:\n");
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
		for (i = 0; i<memoryNum; i++)		//初始化内存中页面
		{
			memory[i].page = -1;				//页面为空用-1表示
			memory[i].wtime = -1;
			memory[i].fwen = 0;
			memory[i].xgai = 0;
		}
		printf("请根据提示选择以下页面置换算法：\n");
		printf("     1:最佳（Optimal)置换算法\n");
		printf("     2:先进先出(FIFO)置换算法\n");
		printf("     3:最近最久未使用(Least Recently Used)置换算法\n");
		printf("     4:改进型Clock置换算法\n");
		printf("     5:页面缓冲算法(PBA)\n");
		printf("*****请选择操作类型(1,2,3,4,5),按其它键结束******\n");
		getchar();
		scanf_s("%c", &c);
		curmemory = 0;
		if (c == '1')			//最佳（Optimal)置换算法
		{
			OPT();
		}
		if (c == '2')           //先进先出(FIFO)置换算法
		{
			FIFO();
		}
		if (c == '3')			//最近最久未使用(Least Recently Used)置换算法
		{
			LRU();
		}
		if (c == '4')			//改进型Clock置换算法
		{
			NewClock();
		}
		if (c == '5')			//页面缓冲算法(PBA)
		{
			PBA();
		}
	} while (c == '1' || c == '2' || c == '3' || c == '4' || c == '5');
	free(page);
	free(memory);
	return 0;
}