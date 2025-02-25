#include <iostream>
#include <pthread.h>
#include <cstring>
#include <format>

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
        std::cout << "error : " << str << std::endl; 
    }

    //主线程
    for (int i = 0; i < 5; i++)
        std::cout << i << '\n';

    std::cout << std::format("tid: {}, main thread id: {}\n", tid, pthread_self());

    std::cout << std::format("tid == main id ?  {}\n", pthread_equal(tid, pthread_self()));

    //让主线程退出，当主进程退出时，不会影响其他正常运行的进程
    pthread_exit(NULL);

    std::cout << "main exit\n"; //不会执行

    return 0;
}