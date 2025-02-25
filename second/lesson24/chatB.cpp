#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>

int main() {
    //1.判断有名管道是否存在
    int ret = access("fifo1", F_OK);
    if (ret == -1) {
        //文件不存在
        std::cout << "管道不存在，创建对应的有名管道\n";
        ret = mkfifo("fifo1", 0664);
        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    ret = access("fifo2", F_OK);
    if (ret == -1) {
        //文件不存在
        std::cout << "管道不存在，创建对应的有名管道\n";
        ret = mkfifo("fifo2", 0664);
        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    //2.以读的方式打开管道fifo1
    int fdr = open("fifo1", O_RDONLY);
    if (fdr == -1) {
        perror("open");
        exit(0);
    }

    std::cout << "打开fifo1成功，等待读取...\n";

    //3.以只写的方式打开管道fifo2
    int fdw = open("fifo2", O_WRONLY);
    if (fdw == -1) {
        perror("open");
        exit(0);
    }

    std::cout << "打开fifo2成功，等待写入...\n";

    char buf[128];

    //4.循环的读写数据
    while (true) {
        //5.读管道数据
        memset(buf, 0, sizeof(buf));
        ret = read(fdr, buf, 128);
        if (ret <= 0) {
            perror("read");
            exit(0);
        }
        std::cout << "buf: " << buf << std::endl;

        memset(buf, 0, sizeof(buf));
        // 获取标准输入的数据
        fgets(buf, 128, stdin);
        //写数据
        ret = write(fdw, buf, strlen(buf));
        if (ret == -1) {
            perror("write");
            exit(0);
        }
    }

    //6.关闭文件描述符
    close(fdw);
    close(fdr);

    return 0;
}