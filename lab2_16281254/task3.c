#include<stdio.h>
#include<unistd.h>
int main(){
	pid_t p1,p2;
	p1=fork();
	if(p1<0){
		printf("pid: %d,error!\n！",getpid());	
	}
	if(p1==0)
	{
		p2=fork();
		if(p2>0)
			fork();
		printf("pid: %d，parent pid: %d\n",getpid(),getppid());
		sleep(1);


	}
	if(p1>0)
	{
		fork();
		printf("pid: %d，parent pid: %d\n",getpid(),getppid());
		sleep(1);
	}
	//sleep(60);
	return 0;
}
