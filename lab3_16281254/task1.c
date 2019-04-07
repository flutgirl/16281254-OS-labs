#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<fcntl.h>
int main(){
	sem_t *P1_signal,*P2_signal,*P3_signal;// 信号量声明，主函数中的进程是P1
	pid_t p2,p3,p4; 
	P1_signal=sem_open("P1_signal",O_CREAT,0666,0);//创建并初始化信号灯
	P2_signal=sem_open("P2_signal",O_CREAT,0666,0);
	P3_signal=sem_open("P3_signal",O_CREAT,0666,0);
	p2=fork();//创建进程P2
	if(p2<0){
		perror("Error in creating process p2！");
	}
	if(p2==0){//P1子进程P2 
		sem_wait(P1_signal);//互斥信号量
		printf("I am the process P2!\n");
		sem_post(P1_signal);
		sem_post(P2_signal);
	}
	if(p2>0){//父进程P1
		p3=fork();//创建进程P3
		if(p3<0){
			perror("Error in creating process p3！");
		}
		if(p3==0){//P1子进程P3
			sleep(1);
			sem_wait(P1_signal);//互斥信号量
			printf("I am the process P3!\n");
			sem_post(P1_signal);
			sem_post(P3_signal);
		}
		if(p3>0){//父进程P1
			printf("I am the process P1!\n");
			sem_post(P1_signal);
			p4=fork();
			if(p4<0){
				perror("Error in creating process p4！");
			}
			if(p4==0){//P1子进程P4
				sem_wait(P2_signal);//等待进程P2
				sem_wait(P3_signal);//等待进程P3
				printf("I am the process P4!\n");
				sem_post(P2_signal);
				sem_post(P3_signal);
			}
			sleep(2);
		}
	}
	sem_close(P1_signal);//关闭有名信号灯
	sem_close(P3_signal);
	sem_close(P2_signal);
	sem_unlink("P1_signal");//从系统中删除信号灯
	sem_unlink("P2_signal");
	sem_unlink("P3_signal");
	return 0;
}
