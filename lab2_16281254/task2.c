#include<stdio.h>
#include<unistd.h>
int main(){
	pid_t pid,fpid;//进程id号
	fpid=fork();//创建子进程
	if(fpid<0){//如果为-1表示创建失败
		printf("创建子进程出错！");}
	if(fpid==0)//成功创建一个子进程，对于子进程来说返回值为0
	{
		int ret;//接收函数返回值
		ret = execl("/usr/bin/vi","vi",NULL);
		if(ret==-1)
			perror("excel l error");
	} 
	if(fpid>0)//成功创建一个子进程，对于父进程来说返回子进程ID
	{
		printf("子进程的pid为%d\n",fpid);
		printf("父进程的pid为%d\n",getpid());
		/* 父进程执行空循环 */
		while(1)
			sleep(2);
	}
	return 0;
}
