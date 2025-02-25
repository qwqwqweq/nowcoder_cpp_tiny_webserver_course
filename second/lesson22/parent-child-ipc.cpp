#include <unistd.h>
#include <iostream>
#include <format>
#include <cstring>
#include <string>
#include <sstream>
#include <sys/wait.h>

int main() {
    //创建管道
    int fd[2];
    int ret = pipe(fd);
    if (ret == -1) {
        perror("pipe");
        exit(0);
    }

    //创建子进程
    pid_t pid = fork();
    if (pid > 0) {
        //父进程
        //关闭写端
        close(fd[1]);

        //从管道中读取
        int len = -1;
        char buf[1024]{};
        std::string output;
        while ((len = read(fd[0], buf, sizeof(buf) - 1)) > 0) {
            // std::cout << buf << std::endl;
            output += buf;
            memset(buf, 0, sizeof(buf));
        }
        wait(NULL);
        //过滤数据输出
        std::istringstream iss(output);
        std::string line;
        std::string filter = "root";
        while (std::getline(iss, line))
            if (line.find(filter) != std::string::npos)
                std::cout << line << std::endl;

    } else if (!pid) {
        //子进程
        //关闭读端
        close(fd[0]);
        //文件描述符的重定向 stdout_fileno -> fd[1]
        dup2(fd[1], STDOUT_FILENO);
        //执行 ps aux
        execlp("ps", "ps", "aux", NULL);
        perror("execlp");
        exit(0);
    } else {
        perror("fork");
        exit(0);
    }

    return 0;
}