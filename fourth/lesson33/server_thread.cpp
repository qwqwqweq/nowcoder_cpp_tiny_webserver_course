#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>

struct sockInfo {
    int fd; //通信的文件描述符
    pthread_t tid; //线程号
    sockaddr_in addr;
};

sockInfo sockinfos[128];

void *working(void *arg) {
    //子线程和客户端通信 cfd 客户端信息 线程号
    //获取客户端的信息
    sockInfo *pinfo = (sockInfo *)arg;
    char clientIP[16]{};
    inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short port = ntohs(pinfo->addr.sin_port);
    std::cout << "client ip is : " << clientIP << " port is : " << port << std::endl;

    //接受客户端发来的数据
    char recvBuf[1024]{};
    while (true) {
        int len = read(pinfo->fd, &recvBuf, sizeof(recvBuf));
        if (len == -1) {
            std::cerr << "read" << std::endl;
            exit(-1);
        } else if (len > 0) {
            std::cout << "recv client data : " << recvBuf << std::endl;
        } else {
            std::cout << "client closed..." << std::endl;
            break;
        }
        write(pinfo->fd, recvBuf, strlen(recvBuf));
    }
    close(pinfo->fd);

    return NULL;
}

int main() {
    //创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        std::cerr << "socket" << std::endl;
        exit(-1);
    }

    //绑定
    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);
    int ret = bind(lfd, (sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1) {
        std::cerr << "bind" << std::endl;
        exit(-1);
    }

    //监听
    ret = listen(lfd, 128);
    if (ret == -1) {
        std::cerr << "listen" << std::endl;
        exit(-1);
    }

    //初始化数据
    int max = sizeof(sockinfos) / sizeof(sockinfos[0]);
    for (int i = 0; i < max; i++) {
        bzero(&sockinfos[i], sizeof(sockinfos[i]));
        sockinfos[i].fd = -1;
        sockinfos[i].tid = -1;
    }

    //不断等待客户端连接，一旦一个客户端连接进来，就创建一个子线程通信
    while (true) {
        //接受连接
        sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int cfd = accept(lfd, (sockaddr *)&cliaddr, &len);
        if (cfd == -1) {
            std::cerr << "accept" << std::endl;
            exit(-1);
        }

        
        sockInfo *pinfo;
        for (int i = 0; i < max; i++) {
            //从数组中找到一个可以用的sockInfo元素
            if (sockinfos[i].fd == -1) {
                pinfo = &sockinfos[i];
                break;
            }
            if (i == max - 1) {
                sleep(1);
                i--;
            }
        }
        pinfo->fd = cfd;
        memcpy(&pinfo->addr, &cliaddr, len);
        //创建子线程
        pthread_create(&pinfo->tid, NULL, working, pinfo);
        pthread_detach(pinfo->tid);
    }

    close(lfd);

    return 0;
}