#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

int main() {
    //1.判断文件是否存在
    int ret = access("test", F_OK);
    if (ret == -1) {
        std::cout << "管道不存在，创建管道\n";
        //2.创建管道
        ret = mkfifo("test", 0664);

        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    //3.以只写的方式打开管道
    int fd = open("test", O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(0);
    }

    //4.写数据
    for (int i = 0; i < 100; i++) {
        std::ostringstream os;
        os << "hello, " << i << '\n';
        std::string s = os.str();
        std::cout << "write data: " << s << '\n';
        write(fd, s.c_str(), s.size());
        sleep(1);
    }

    close(fd);

    return 0;
}