//匿名映射 不需要文件实体

#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

int main() {
    //1 创建匿名内存映射区
    int len = 4096;
    void *ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    //父子进程间通信
    pid_t pid = fork();

    if (pid > 0) {
        //父进程
        strcpy((char *)ptr, "helloworld");
        wait(NULL);
    } else {
        //子进程
        sleep(1);
        std::cout << static_cast<char *>(ptr) << std::endl;
    }

    //释放内存映射区
    int ret = munmap(ptr, len);
    if (ret == -1) {
        perror("munmap");
        exit(0);
    }

    return 0;
}