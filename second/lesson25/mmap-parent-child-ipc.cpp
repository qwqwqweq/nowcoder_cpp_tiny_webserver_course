#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/wait.h>

int main() {

    //1 打开一个文件
    int fd = open("test.txt", O_RDWR);
    int size = lseek(fd, 0, SEEK_END); // 获取文件大小

    //2 创建内存映射区
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    //3 创建子进程
    pid_t pid = fork();
    if (pid > 0) {
        //父进程
        std::cout << "I am parent\n";
        wait(NULL);
        std::cout << "子进程结束\n";
        char buf[64];
        strcpy(buf, (char *)ptr);
        std::cout << "read data: " << buf << std::endl;
        
    } else {
        //子进程
        std::cout << "I am son!!\n";
        strcpy((char *)ptr, "nihao a, parent!!!");
        std::cout << "write data\n";
    }

    //关闭内核映射区
    munmap(ptr, size);


    return 0;
}