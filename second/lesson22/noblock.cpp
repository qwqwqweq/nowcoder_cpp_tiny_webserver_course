#include <unistd.h>
#include <iostream>
#include <format>
#include <cstring>
#include <fcntl.h>

//设置管道非阻塞
//int flags = fcntl(fd[0], F_GETFL) 获取原来的flag
//flags |= O_NONBLOCK 修改flag
//fcntl(fd[0], F_SETFL, flags) 设置新的flag
int main() {
    //在fork前创建管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        exit(0);
    }

    pid_t pid = fork();
    if (pid > 0) {
        //父进程
        std::cout << std::format("i am parent process, pid: {}\n", getpid());
        char buf[1024]{};
        //关闭写端
        close(pipefd[1]);

        int flags = fcntl(pipefd[0], F_GETFL);
        flags |= O_NONBLOCK;
        fcntl(pipefd[0], F_SETFL, flags);

        while (true) {
            //从管道的读取端读取数据
            int len = read(pipefd[0], buf, sizeof(buf));
            std::cout << std::format("len: {}\n", len);
            std::cout << std::format("parent recv: {}, pid: {}\n", buf, getpid());
            memset(buf, 0, sizeof(buf));
            sleep(1);
        }
    } else {
        //子进程
        std::cout << std::format("i am child process, pid: {}\n", getpid());
        char buf[1024]{};
        //关闭读端
        close(pipefd[0]);
        while (true) {
            //向管道中写入数据
            char *str = "hello, i am child";
            write(pipefd[1], str, strlen(str));
            sleep(5);
        }
        
    }

    return 0;
}