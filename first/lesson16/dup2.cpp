#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <cstring>

int main() {
    int fd = open("1.txt", O_RDWR | O_CREAT, 0664);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    int fd1 = open("2.txt", O_RDWR | O_CREAT, 0664);
    if (fd1 == -1) {
        perror("open");
        return -1;
    }

    std::cout << fd << ' ' << fd1 << '\n';

    int fd2 = dup2(fd, fd1); //fd1->1.txt
    if (fd2 == -1) {
        perror("dup2");
        return -1;
    }

    char *str = "hello, dup2\n";
    int len = write(fd2, str, strlen(str));

    if (len == -1) {
        perror("write");
        return -1;
    }

    std::cout << fd << ' ' << fd1 << ' ' << fd2 << std::endl;

    close(fd);
    close(fd1);

    return 0;
}