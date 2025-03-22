#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/un.h>

int main() {
    unlink("server.sock");

    //创建监听的套接字
    int lfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (lfd == -1) {
        std::cerr << "socket" << std::endl;
        exit(-1);
    }

    //绑定本地套接字文件
    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "server.sock");
    int ret = bind(lfd, (sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        std::cerr << "bind" << std::endl;
        exit(-1);
    }

    //监听
    ret = listen(lfd, 100);
    if (ret == -1) {
        std::cerr << "listen" << std::endl;
        exit(-1);
    }

    //等待客户端连接
    sockaddr_un cliaddr;
    socklen_t len = sizeof(cliaddr);
    int cfd =accept(lfd, (sockaddr *)&cliaddr, &len);
    if (cfd == -1) {
        std::cerr << "accept" << std::endl;
        exit(-1);
    }

    std::cout << "client socket filename: " << cliaddr.sun_path << std::endl;

    //通信
    while (true) {
        char buf[128]{};
        int len = recv(cfd, buf, sizeof(buf), 0);
        if (len == -1) {
            std::cerr << "recv" << std::endl;
        } else if (!len) {
            std::cout << "client closed..." << std::endl;
            break;
        } else if (len > 0) {
            std::cout << "client say: " << buf << std::endl;
            send(cfd, buf, sizeof(buf), 0);
        }
    }

    close(cfd);
    close(lfd);

    return 0;
}