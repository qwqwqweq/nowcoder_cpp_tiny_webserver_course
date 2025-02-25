#include <unistd.h>
#include <iostream>
#include <format>

int main() {


    //创建一个子进程，在子进程中执行exec函数族中的函数

    pid_t pid = fork();

    if (pid > 0) {
        //父进程
        std::cout << "I am parent process, pid : " << getpid() << '\n';
        sleep(1);
    } else {
        //子进程
        // execl("hello", "hello", NULL);

        execl("/usr/bin/ps", "ps", "aux", NULL);

        std::cout << "I am child process\n";
    }

    for (int i = 0; i < 3; i++)
        std::cout << std::format("i = {}, pid = {}\n", i, getpid());

    return 0;
}