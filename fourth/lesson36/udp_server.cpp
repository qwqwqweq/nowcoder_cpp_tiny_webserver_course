#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int main() {
    //创建一个通信的socket
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        std::cerr << "socket\n";
        exit(-1);
    }

    //绑定
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(fd, (sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        std::cerr << "bind\n";
        exit(-1);
    }

    //通信
    while (true) {
        //接受数据
        char buf[128]{};
        char ipbuf[16]{};
        sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int num = recvfrom(fd, buf, sizeof(buf), 0, (sockaddr *)&cliaddr, &len);
        if (num == -1) {
            std::cerr << "recvfrom\n";
            exit(-1);
        }
        std::cout << "client ip : " << inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)) << " port: " << ntohs(cliaddr.sin_port) << std::endl;

        std::cout << "client say: " << buf << std::endl;

        //发送数据
        sendto(fd, buf, strlen(buf) + 1, 0, (sockaddr *)&cliaddr, sizeof(cliaddr));
    }

    close(fd);

    return 0;
}