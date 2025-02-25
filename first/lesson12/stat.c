#include <sys/stat.h>
#include <stdio.h>

int main() {

    struct stat statbuf;

    int ret = stat("a.txt", &statbuf);

    if (ret == -1) {
        perror("stat");
        return -1;
    }

    printf("size:%ld", statbuf.st_size);

    return 0;
}