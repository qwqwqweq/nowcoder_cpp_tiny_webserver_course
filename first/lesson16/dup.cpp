#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <cstring>

int main() {
    int fd = open("a.txt", O_RDWR | O_CREAT, 0664);
    int fd1 = dup(fd);

    if (fd1 == -1) {
        perror("dup");
        return -1;
    }

    std::cout << fd << ' ' << fd1 << '\n';

    close(fd);

    char *str = "hello, world";
    int ret = write(fd1, str, strlen(str));

    if (ret == -1) {
        perror("write");
        return -1;
    }
    close(fd1);

    return 0;
}