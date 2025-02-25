#include <iostream>
#include <cstring>
#include <pthread.h>
#include <format>
#include <unistd.h>

void *callback(void *arg) {
    std::cout << "child thread id: " << pthread_self() << std::endl;
    for (int i = 0; i < 5; i++)
        std::cout << "child :" << i << std::endl;  
    return NULL;
}

int main() {
    //创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);

    if (ret) {
        char *str = strerror(ret);
        std::cout << "error : " << std::endl;
    }

    //取消线程
    pthread_cancel(tid);

    for (int i = 0; i < 5; i++)
        std::cout << i << std::endl;

    //输出主线程和子线程的id
    std::cout << std::format("tid : {}, main thread id: {}\n", tid, pthread_self());

    pthread_exit(NULL);

    return 0;
}