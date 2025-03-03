#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include <algorithm>

int main() {
    //创建一个通信的socket
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        std::cerr << "socket\n";
        exit(-1);
    }

    //服务器地址信息
    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);

    //通信
    int num = 0;
    while (true) {
        //发送数据
        char sendbuf[128]{};
        std::ostringstream os;
        os << "hello, i am client " << num++ << std::endl;
        std::string tmp = os.str();
        std::copy(tmp.begin(), tmp.end(), sendbuf);
        sendbuf[tmp.size()] = '\0';
        sendto(fd, sendbuf, strlen(sendbuf) + 1, 0, (sockaddr *)&saddr, sizeof(saddr));

        //接收数据
        recvfrom(fd, sendbuf, sizeof(sendbuf), 0, NULL, NULL);
        std::cout << "server say: " << sendbuf << std::endl;
        sleep(1);
    }

    close(fd);

    return 0;
}