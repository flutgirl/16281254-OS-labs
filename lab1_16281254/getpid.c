#include <unistd.h>
#include <stdio.h>
int main()
{
    pid_t pid;
    pid = getpid();
    printf("pid = %d \n",pid);
    return 0;
}

