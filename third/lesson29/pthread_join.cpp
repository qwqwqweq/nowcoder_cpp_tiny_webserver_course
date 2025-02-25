#include <iostream>
#include <pthread.h>
#include <cstring>
#include <format>
#include <unistd.h>
int value = 10;
void *callback(void *arg) {
    std::cout << "child thread id: " << pthread_self() << std::endl;
    // sleep(3);
    // return NULL;
    
    pthread_exit((void *)&value); //等价于 return (void *)&value;
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

    //主线程调用pthread_join()回收子线程的资源
    int *thread_retval;
    ret = pthread_join(tid, (void **)&thread_retval);
    if (ret) {
        char *str = strerror(ret);
        std::cout << "error : " << str << std::endl; 
    }
    std::cout << "exit data: " << *thread_retval << std::endl;
    std::cout << "回收子线程资源成功\n";

    //让主线程退出，当主进程退出时，不会影响其他正常运行的进程
    pthread_exit(NULL);


    return 0;
}