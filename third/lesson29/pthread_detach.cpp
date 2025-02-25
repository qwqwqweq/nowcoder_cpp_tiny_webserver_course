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
    //创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);

    if (ret) {
        char *str = strerror(ret);
        std::cout << "error : " << std::endl;
    }

    //输出主线程和子线程的id
    std::cout << std::format("tid : {}, main thread id: {}\n", tid, pthread_self());

    //设置子线程分离，子线程分离后，子线程结束时对应的资源就不需要主线程释放
    ret = pthread_detach(tid);
    if (ret) {
        char *str1 = strerror(ret);
        std::cout << "error1 : " << str1 << std::endl;
    }

    //设置分离后，对分离的子线程进行连接 pthread_join()
    ret = pthread_join(tid, NULL);
    if (ret) {
        char *str2 = strerror(ret);
        std::cout << "error2 : " << str2 << std::endl;
    }

    pthread_exit(NULL);

    return 0;
}