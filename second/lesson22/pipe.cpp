    //    #include <unistd.h>

    //    int pipe(int pipefd[2]);

#include <unistd.h>
#include <iostream>
#include <format>
#include <cstring>

//子进程发数据给父进程，父进程读取到数据输出
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
        while (true) {
            //从管道的读取端读取数据
            int len = read(pipefd[0], buf, sizeof(buf));
            std::cout << std::format("parent recv: {}, pid: {}\n", buf, getpid());
            //向管道中写入数据
            // char *str = "hello, i am parent";
            // write(pipefd[1], str, strlen(str));
            // sleep(1);
        }
    } else {
        //子进程
        std::cout << std::format("i am child process, pid: {}\n", getpid());
        char buf[1024]{};
        //关闭读端
        close(pipefd[0]);
        while (true) {
            //向管道中写入数据
            char *str = "hello, i am child\n";
            write(pipefd[1], str, strlen(str));
            // sleep(1);
            // //从管道的读取端读取数据
            // int len = read(pipefd[0], buf, sizeof(buf));
            // std::cout << std::format("child recv: {}, pid: {}\n", buf, getpid());
        }
        
    }

    return 0;
}