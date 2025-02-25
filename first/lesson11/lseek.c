#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("hello.txt", O_RDWR);

    if (fd == -1) {
        perror("open");
        return -1;
    }

    //扩展文件的长度
    lseek(fd, 100, SEEK_END);

    write(fd, "", 1);

    close(fd);

    return 0;
}