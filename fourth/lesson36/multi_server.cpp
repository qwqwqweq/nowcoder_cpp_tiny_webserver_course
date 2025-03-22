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

    //设置多播属性，设置外出接口
    in_addr imr_multiaddr;
    //初始化多播地址
    inet_pton(AF_INET, "239.0.0.10", &imr_multiaddr.s_addr);
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &imr_multiaddr, sizeof(imr_multiaddr));

    //初始化客户端的地址信息
    sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(9999);
    inet_pton(AF_INET, "239.0.0.10", &cliaddr.sin_addr.s_addr); 

    //通信
    int num = 0;
    while (true) {
        //发送数据
        std::string buf = std::format("hello, client...{}\n", num++);
        sendto(fd, buf.c_str(), buf.size(), 0, (sockaddr *)&cliaddr, sizeof(cliaddr));
        std::cout << "组播的数据: " << buf << std::endl;
        sleep(1);
    }

    close(fd);

    return 0;
}