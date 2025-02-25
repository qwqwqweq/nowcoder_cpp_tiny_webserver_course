#include <iostream>
#include <signal.h>

int main() {

    //创建一个信号集
    sigset_t set;

    //清空信号集的内容
    sigemptyset(&set);

    //判断SIGINT是否在信号集set里
    int ret = sigismember(&set, SIGINT);
    if (!ret) {
        std::cout << "SIGINT 不阻塞\n";
    } else if (ret == 1) {
        std::cout << "SIGINT 阻塞\n";
    } else {
        std::cout << "error\n";
    }

    //添加几个信号到信号集中
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);


    //判断SIGINT是否在信号集set里
    ret = sigismember(&set, SIGINT);
    if (!ret) {
        std::cout << "SIGINT 不阻塞\n";
    } else if (ret == 1) {
        std::cout << "SIGINT 阻塞\n";
    } else {
        std::cout << "error\n";
    }
    
    //判断SIGQUIT是否在信号集set里
    ret = sigismember(&set, SIGQUIT);
    if (!ret) {
        std::cout << "SIGQUIT 不阻塞\n";
    } else if (ret == 1) {
        std::cout << "SIGQUIT 阻塞\n";
    } else {
        std::cout << "error\n";
    }


    //从信号集中删除一个信号
    sigdelset(&set, SIGQUIT);

    //判断SIGQUIT是否在信号集set里
    ret = sigismember(&set, SIGQUIT);
    if (!ret) {
        std::cout << "SIGQUIT 不阻塞\n";
    } else if (ret == 1) {
        std::cout << "SIGQUIT 阻塞\n";
    } else {
        std::cout << "error\n";
    }

    return 0;
}