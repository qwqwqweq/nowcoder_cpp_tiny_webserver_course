#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int srcfd = open("english.txt", O_RDONLY);
    if (srcfd == -1) {
        perror("open");
        return -1;
    }

    int destfd = open("copy.txt", O_WRONLY |  O_CREAT, 0664);
    if (destfd == -1) {
        perror("open");
        return -1;
    }

    char buf[1024];
    int len = 0;
    while ((len = read(srcfd, buf, sizeof(buf))) > 0) {
        write(destfd, buf, len);
    }

    close(destfd);
    close(srcfd);

    return 0;
}