#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <format>

int main() {
    //打开管道
    int fd = open("test", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(0);
    }

    //读数据
    while (true) {
        std::string s(1024, '\0');
        int len = read(fd, s.data(), s.size());
        if (!len) {
            std::cout << "写端断开了...\n";
            break;
        }
        s.resize(len);
        std::cout << std::format("recv string: {}\n", s) << std::endl;
     }

    close(fd);

    return 0;
}