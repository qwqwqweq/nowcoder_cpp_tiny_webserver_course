#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    //1 创建socket 用于监听
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }

    //2 绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET, "172.22.8.133", saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));

    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    //3 监听
    ret = listen(lfd, 8);

    if (ret == -1) {
        perror("listen");
        exit(-1);
    }

    //4接受客户端连接
    struct sockaddr_in clientaddr;
    socklen_t len1 = sizeof(clientaddr);
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len1);

    if (cfd == -1) {
        perror("accept");
        exit(-1);
    }

    //输出客户端信息
    char clientIP[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);

    std::cout << "client ip is " << clientIP << ", port is " << clientPort << std::endl;

    //5 通信
    //获取客户端数据
    char recvBuf[1024]{};
    while (true) {
        int len = read(cfd, recvBuf, sizeof(recvBuf));
        if (len == -1) {
            perror("read");
            exit(-1);
        } else if (len > 0) {
            std::cout << "recv client data: " << recvBuf << std::endl;
        } else if (len == 0) {
            //表示客户端断开连接
            std::cout << "client closed...\n";
            break;
        }
    
        //给客户端发送数据
        char *data = "hello, i am server.";
        write(cfd, data, strlen(data));
    }

    //关闭文件描述符
    close(cfd);
    close(lfd);

    return 0;
}