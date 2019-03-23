#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(){
	pid_t p1,p2,p3;int i;
	p1=fork();
	if(p1<0){
		printf("pid: %d,error!\n！",getpid());	
	}
	if(p1==0)
	{
		p2=fork();
		if(p2>0)
			p3=fork();
		while(i<99){
		printf("pid: %d，parent pid: %d\n",getpid(),getppid());
		i++;
		sleep(1);
		if(p3>0 && p2>0 && i>10)
			{int*ptr=NULL;*ptr=0;}}
	}
	if(p1>0)
	{
		fork();
		while(1){
		printf("pid: %d，parent pid: %d\n",getpid(),getppid());
		sleep(1);}
	}
	//sleep(60);
	return 0;
}
