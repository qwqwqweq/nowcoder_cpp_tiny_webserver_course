//使用内存映射实现文件拷贝功能

#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main() {

    //1 对原始的文件进行内存映射
    int fd = open("english.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(0);
    }

    //获取原始文件的大小
    int size = lseek(fd, 0, SEEK_END);

    //2 创建一个新文件(扩展该文件)
    int fd1 = open("cpy.txt", O_RDWR | O_CREAT, 0664);
    if (fd1 == -1) {
        perror("open");
        exit(0);
    }

    //对新文件进行拓展
    truncate("cpy.txt", size);
    write(fd1, " ", 1);

    //3 分别做内存映射
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    void *ptr1 = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    if (ptr1 == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    //内存拷贝
    memcpy(ptr1, ptr, size);

    //释放资源
    munmap(ptr1, size);
    munmap(ptr, size);

    close(fd1);
    close(fd);

    return 0;
}