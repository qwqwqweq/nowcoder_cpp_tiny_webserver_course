#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>

int main() {
    int fd = open("chat.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(0);
    } 
    ftruncate(fd, 128);
    int size = lseek(fd, 0, SEEK_END);

    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    while (true) {
        //读
        char buf[128];
        strcpy(buf, (char *)ptr);
        if (!strcmp(buf, "#")) {
            std::cout << "停止读入\n";
            break;
        }
        std::cout << "read data: " << buf << std::endl;
        sleep(10);

        // //写
        // memset(buf, 0, sizeof(buf));
        // std::cin >> buf;
        // strcpy((char *)ptr, buf);
        // sleep(2);
    }

    munmap(ptr, size);


    return 0;
}