#include <unistd.h>
#include <stdio.h>
#include <sched.h>

int main() {
    pid_t pid = fork();

    //判断是父进程还是子进程
    if (pid > 0) {
        printf("i am parent process, pid : %d, ppid : %d\n", getpid(), getppid());
    } else if (!pid) {
        // 子进程
        sleep(2);
        printf("i am child process, pid : %d, ppid : %d\n", getpid(), getppid());
       
    } 

    for (int i = 0; i < 3; i++) {
        printf("i : %d, pid : %d\n", i, getpid());
        // sleep(1);
    }

    return 0;
} 