#include <unistd.h>
#include <stdio.h>
int main()
{
    pid_t pid;
    pid = getpid();
    asm volatile(
            "mov $0x14,%%eax\n\t" /* 将系统调用号20放入eax中。 */
            "int $0x80\n\t" /* 中断向量号0x80，即128。int 128 执行系统调用。 */
            "mov %%eax,%0\n\t" /* 返回值保存在eax中，将它赋值给pid */
            : "=m" (pid)   
        );  
    printf("pid = %d \n",pid);
    return 0;
}

