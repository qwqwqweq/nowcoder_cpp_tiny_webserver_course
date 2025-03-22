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

    //客服端绑定本地的IP和端口
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(fd, (sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        std::cerr << "bind" << std::endl;
        exit(-1);
    }

    //加入到多播组
    ip_mreq op;
    inet_pton(AF_INET, "239.0.0.10", &op.imr_multiaddr.s_addr);
    op.imr_interface.s_addr = INADDR_ANY;
    setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &op, sizeof(op));

    //通信
    while (true) {
        //接收数据
        char buf[128]{};
        recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
        std::cout << "server say: " << buf << std::endl;
    }

    close(fd);

    return 0;
}