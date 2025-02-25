#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>

int main() {
    //判断文件是否存在
    int ret = access("fifo1", F_OK);
    if (ret == -1) {
        std::cout << "管道不存在，创建管道\n";
        ret = mkfifo("fifo1", 0664);

        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }
    return 0;
}