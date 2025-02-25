#include <iostream>
#include <signal.h>
#include <cstring>

//编写一个程序，把所有的常规信号（1 - 31）未决状态打印到屏幕
int main() {
    //设置2 3 号信号阻塞
    sigset_t set;
    sigemptyset(&set);

    //将2 3 号信号添加到信号集中
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    //修改内核中的阻塞信号集
    sigprocmask(SIG_BLOCK, &set, NULL);

    int num = 0;
    while (true) {
        num++;
        //获取当前的未决信号集的数据
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);

        //遍历前32位
        for (int i = 1; i <= 31; i++) {
            if (sigismember(&pendingset, i) == 1)
                std::cout << "1";
            else if (sigismember(&pendingset, i) == 0)
                std::cout << "0";
            else {
                perror("sigismember");
                exit(0);
            }
        }
        std::cout << std::endl;
        sleep(1);
        if (num == 10) {
            //解除阻塞
            sigprocmask(SIG_UNBLOCK, &set, NULL);
        }
    }


    return 0;
}