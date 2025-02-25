//使用多线程实现买票的案列
//有3个窗口，一共是100张票

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <format>
#include <string>

//全局变量，所有线程共享这一份资源
int tickets = 100;

void *sellticket(void *arg) {
    //卖票
    while (tickets > 0) {
        usleep(3000);
        std::cout << std::format("{} 正在卖第 {} 张门票", pthread_self(), tickets) << std::endl;
        tickets--; 
    }
    return NULL;
}

int main() {
    //创建3个子线程
    pthread_t tid[3];
    for (int i = 0; i < 3; i++) {
        pthread_create(&tid[i], NULL, sellticket, NULL);
    }

    //回收子线程的资源，阻塞
    for (int i = 0; i < 3; i++) {
        pthread_join(tid[i], NULL);
    }

    //设置线程分离
    // for (int i = 0; i < 3; i++) {
    //     pthread_detach(tid[i]);
    // }

    pthread_exit(NULL); //退出主线程

    return 0;
}