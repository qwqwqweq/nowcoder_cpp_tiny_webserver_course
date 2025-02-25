#include <pthread.h>
#include <iostream>
#include <cstring>
#include <unistd.h>

void *callback(void *arg) {
    std::cout << "child thread...\n";
    std::cout << "arg value: " << *(int *)arg << std::endl;
    return NULL;
}

int main() {
    pthread_t tid;
    int num = 10;
    //创建一个子线程
    int ret = pthread_create(&tid, NULL, callback, (void *)&num);
    if (ret) {
        char *errstr = strerror(ret);
        std::cout << errstr << std::endl;
    } 

    for (int i = 0; i < 5; i++) {
        std::cout << i << std::endl;
    }

    sleep(1);

    return 0;
}