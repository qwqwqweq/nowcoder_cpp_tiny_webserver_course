#include <sys/stat.h>
#include <stdio.h>

int main() {
    int ret = mkdir("aaa", 0777);

    if (ret == -1) {
        perror("mkdir");
        return -1;
    }

    return 0;
}