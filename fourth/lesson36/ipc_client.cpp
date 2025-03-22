#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/un.h>

int main() {
    unlink("client.sock");
    //创建套接字
    int cfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (cfd == -1) {
        std::cerr << "socket" << std::endl;
        exit(-1);
    }

    //绑定本地套接字文件
    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "client.sock");
    int ret = bind(cfd, (sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        std::cerr << "bind" << std::endl;
        exit(-1);
    }

    //连接服务器
    sockaddr_un seraddr;
    seraddr.sun_family = AF_UNIX;
    strcpy(seraddr.sun_path, "server.sock");
    ret = connect(cfd, (sockaddr *)&seraddr, sizeof(seraddr));
    if (ret == -1) {
        std::cerr << "connect" << std::endl;
        exit(-1);
    }

    //通信
    int num = 0;
    while (true) {
        //发送数据
        char buf[128]{};
        sprintf(buf, "hello, i am client %d\n", num++);
        send(cfd, buf, strlen(buf) + 1, 0);
        std::cout << "client say:" << buf << std::endl;

        //接收数据
        int len = recv(cfd, buf, sizeof(buf), 0);
        if (len == -1) {
            std::cerr << "recv" << std::endl;
        } else if (!len) {
            std::cout << "server closed..." << std::endl;
            break;
        } else if (len > 0) {
            std::cout << "server say: " << buf << std::endl;
        }
        sleep(1);
    }

    close(cfd);

    return 0;
}