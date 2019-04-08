#include <stdio.h>
#include <unistd.h>     //for pipe()
#include <string.h>     //for memset()
#include <stdlib.h>     //for exit()
int main()
{
    int fd[2];
    char buf[100]={0};
	pid_t p1;
    //创建管道
	if(-1 == pipe(fd)){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
	p1=fork();//创建子进程
	if(p1< 0){//创建失败
		perror("fork");
		exit("EXIT_FAILURE");
	}
	if(p1==0){//子进程
		close(fd[1]);
		while(1){
			if(read(fd[0],buf,sizeof(buf))>0)
					printf("the received message in the child:%s\n",buf);
			else
					printf("none in the child\n");
			sleep(2);		
			if(strcmp(buf,"end")==0)
					break;
			memset(buf,0,sizeof(buf));
		}
	}
	if(p1>0){//父进程
		close(fd[0]);
		while(1){
			printf("this is the father,please input:");
			scanf("%s",buf);
			write(fd[1],buf,strlen(buf));
			if(strcmp(buf,"end")==0)
				break;
		}	
	}
    return 0;
}
