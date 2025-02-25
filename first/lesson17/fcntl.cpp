#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

int main() {
    //1.复制文件描述符
    // int fd = open("1.txt", O_RDONLY);

    // int fd1 = fcntl(fd, F_DUPFD);

    //2.修改或者获取文件状态flag
    int fd = open("1.txt", O_RDWR);

    //获取文件描述符状态flag
    int flag = fcntl(fd, F_GETFL);

    //修改文件描述符状态flag, 给flag加入O_APPEND加入这个标记
    if (fd == -1) {
        perror("open");
        return -1;
    }
    flag |= O_APPEND;

    int ret = fcntl(fd, F_SETFL, flag);

    if (ret == -1) {
        perror("fcntl");
        return -1;
    }

    char *str = "\nnihao\n";
    write(fd, str, strlen(str));

    close(fd);

    return 0;
}