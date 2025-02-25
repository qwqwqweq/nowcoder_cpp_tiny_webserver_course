    //    #include <unistd.h>

    //    pid_t fork(void);
    // 返回两次，一次是在父进程中，一次是在子进程中

#include <unistd.h>
#include <stdio.h>
#include <sched.h>

int main() {
    pid_t pid = fork();

    //判断是父进程还是子进程
    if (pid > 0) {
        printf("pid : %d\n", pid);
        //如果大于0，返回的是创建的子进程的进程号，当前是父进程
        printf("i am parent process, pid : %d, ppid : %d\n", getpid(), getppid());

        for (int i = 0; i < 5; i++) {
            printf("i : %d, pid : %d\n", i, getpid());
            sleep(1);
        }
    } else if (!pid) {
        // 子进程
        printf("i am child process, pid : %d, ppid : %d\n", getpid(), getppid());
        for (int i = 0; i < 5; i++) {
            printf("i : %d, pid : %d\n", i, getpid());
            sleep(1);
        }
    } 

    // for (int i = 0; i < 1000; i++) {
    //     printf("i : %d, pid : %d\n", i, getpid());
    //     // sleep(1);
    // }

    return 0;
} 