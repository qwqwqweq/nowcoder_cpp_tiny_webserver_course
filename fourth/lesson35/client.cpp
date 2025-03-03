#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <format>
#include <cstdlib>

int main() {
    //1 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(-1);
    }

    //2 连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "172.22.8.133", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9999);
    int ret = connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    if (ret == -1) {
        perror("connect");
        exit(-1);
    }

    //3 通信
    //获取服务器数据
    char recvBuf[1024]{};
    int i = 0;
    while (true) {
        //给服务器发送数据
        std::string data;
        getline(std::cin, data);
        write(fd, data.c_str(), data.size());
        int len = read(fd, recvBuf, sizeof(recvBuf));
        if (len == -1) {
            perror("read");
            exit(-1);
        } else if (len > 0) {
            std::cout << "recv server data: " << recvBuf << std::endl;
        } else if (len == 0) {
            //表示客户端断开连接
            std::cout << "server closed...\n";
            break;
        }
    }

    //关闭连接
    close(fd);

    return 0;
}