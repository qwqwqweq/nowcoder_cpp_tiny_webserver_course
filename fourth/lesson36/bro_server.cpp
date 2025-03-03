#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <format>

int main() {
    //创建一个通信的socket
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        std::cerr << "socket\n";
        exit(-1);
    }

    //设置广播属性
    int op = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &op, sizeof(op));

    //创建一个广播的地址
    sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(9999);
    inet_pton(AF_INET, "172.22.15.255", &cliaddr.sin_addr.s_addr); 

    //通信
    int num = 0;
    while (true) {
        //发送数据
        std::string buf = std::format("hello, client...{}\n", num++);
        sendto(fd, buf.c_str(), buf.size(), 0, (sockaddr *)&cliaddr, sizeof(cliaddr));
        std::cout << "广播的数据: " << buf << std::endl;
        sleep(1);
    }

    close(fd);

    return 0;
}