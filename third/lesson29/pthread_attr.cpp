#include <iostream>
#include <cstring>
#include <pthread.h>
#include <format>
#include <unistd.h>

void *callback(void *arg) {
    std::cout << "child thread id: " << pthread_self() << std::endl;
    return NULL;
}

int main() {
    //创建一个线程属性变量
    pthread_attr_t attr;
    //初始化属性变量
    pthread_attr_init(&attr);
    //设置属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    //创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, &attr, callback, NULL);

    //获取线程栈的大小
    size_t size;
    pthread_attr_getstacksize(&attr, &size);
    std::cout << "thread stack size: " << size << std::endl; 

    if (ret) {
        char *str = strerror(ret);
        std::cout << "error : " << std::endl;
    }

    //输出主线程和子线程的id
    std::cout << std::format("tid : {}, main thread id: {}\n", tid, pthread_self());

    //释放线程属性资源
    pthread_attr_destroy(&attr);

    pthread_exit(NULL);

    return 0;
}