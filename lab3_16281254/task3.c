#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include <fcntl.h>
#include<stdio.h>
#include <sys/stat.h>
#include <sched.h>
#include <semaphore.h>

sem_t* empty_mySem = NULL;
sem_t* full_mySem = NULL;

char buf[10];//缓冲区大小为10
short i = 0;
short j = 0;

void*input(){//输入线程
    while(1){
        sem_wait(empty_mySem);//当缓冲区有空间时输入
	printf("输入：");        
	scanf("%c",&buf[i++ % 10]);
        sem_post(full_mySem);//增加缓冲数据资源
    }
}

void*output(){//输出线程
    while(1){
      sem_wait(full_mySem);//当缓冲区有数据时输出
        printf("输出:%c\n",buf[j++ % 10]);
        sleep(1);
       sem_post(empty_mySem);//释放缓冲区空间
    }
}

int main(){
    pthread_t p1,p2;//线程标识符
    empty_mySem = sem_open("inputs", O_CREAT, 0666, 10);//信号量初始化
    full_mySem = sem_open("outputs", O_CREAT, 0666, 0);
    pthread_create(&p1,NULL,input,NULL);//创建线程
    pthread_create(&p2,NULL,output,NULL);
    pthread_join(p1,NULL);//等待线程p1的结束
    pthread_join(p2,NULL);
    sem_close(empty_mySem);//关闭信号量
    sem_close(full_mySem);
    sem_unlink("inputs");//从系统中删除信号量
    sem_unlink("outputs");
    return 0;
}
