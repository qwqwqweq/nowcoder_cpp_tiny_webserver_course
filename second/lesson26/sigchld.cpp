#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void myFun(int num) {
    std::cout << "捕捉到的信号: " << num << std::endl;
    //回收子进程PCB的资源
    // wait(NULL);

    while (true) {
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret > 0) {
            std::cout << "child die, pid = " << ret << std::endl;
        } else if (!ret) {
            //说明还有子进程存货
            break;
        } else if (ret == -1) {
            //没有子进程
            break;
        }
    }
}

int main() {
    //提前设置好阻塞信号集，阻塞SIGCHLD，因为有可能子进程很快结束，父进程还没注册号信号捕捉
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);

    pid_t pid;
    for (int i = 0; i < 20; i++) {
        pid = fork();
        if (!pid)
            break;
    }

    if (pid > 0) {
        //父进程

        //捕捉子进程死亡时发生的SIGCHLD信号
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = myFun;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD, &act, NULL);
        //注册完信号捕捉以后，解除阻塞
        sigprocmask(SIG_UNBLOCK, &set, NULL);
        while (true) {
            std::cout << "partent process pid: " << getpid() << std::endl;
            sleep(2);
        }
    } else {
        //子进程
        std::cout << "child process pid: " << getpid() << std::endl;
    }

    return 0;
}