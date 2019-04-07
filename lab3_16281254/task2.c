#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sched.h>
#define initial 1000 //初始票数

sem_t* mySem = NULL;
int ticketCount = initial;
void*sold(){//售票线程
    int i = 150;
    while(i--){
         //sem_wait(mySem);
        printf("Current ticketCount  number is %d.\n",ticketCount);
        int temp = ticketCount;
         sched_yield();
        temp = temp - 1;
         sched_yield();
        ticketCount = temp;
	//sched_yield();
       // sem_post(mySem);
    }
}

void*returnT(){//退票线程
    int i = 50;
    while(i--){
       // sem_wait(mySem);
        printf("Current ticketCount number is %d.\n",ticketCount);
        int temp = ticketCount;
         sched_yield();
        temp = temp + 1;
         sched_yield();
        ticketCount = temp;
        //sem_post(mySem);
    }
}

int main(){
    pthread_t p1,p2;//线程标识符
    mySem = sem_open("Ticket", O_CREAT, 0666, 1);//初始化信号量
    pthread_create(&p1,NULL,sold,NULL);//创建线程
    pthread_create(&p2,NULL,returnT,NULL);
    pthread_join(p1,NULL);//等待线程P1的结束
    pthread_join(p2,NULL);
    sem_close(mySem);//关闭信号量
    sem_unlink("Ticket");//从系统中删除信号量
    printf("The finall value of ticketCount is %d.\n",ticketCount);
    return 0;
}
