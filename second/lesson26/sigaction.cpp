#include <sys/time.h>
#include <iostream>
#include <signal.h>

//过3秒以后，每隔2秒定时一次

//typedef void (*sighandler_t)(int); 函数指针，int类型的参数表示捕捉到的信号的值

void myalarm(int num) {
    std::cout << "捕捉到了信号的编号是：" << num << std::endl;
    std::cout << "xxxxxxx\n";
}

int main() {

    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myalarm;
    sigemptyset(&act.sa_mask); //清空临时阻塞信号集

    //注册信号捕捉
    int ret1 = sigaction(SIGALRM, &act, NULL);
    if (ret1 == -1) {
        perror("signal");
        exit(0);
    }

    itimerval new_value;
    //设置间隔的时间
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    //设置延迟的时间
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &new_value, NULL); //非阻塞
    std::cout << "定时器开始了...\n";
    if (ret == -1) {
        perror("setitimer");
        exit(0);
    } 

    // getchar();
    for (; ;) ;

    return 0;
}