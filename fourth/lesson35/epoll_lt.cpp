#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>

int main() {
    //创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    //绑定
    bind(lfd, (sockaddr *)&saddr, sizeof(saddr));

    //监听
    listen(lfd, 8);

    //调用epoll_create()创建一个epoll实例
    int epfd = epoll_create(100);

    //将监听的文件描述符相关的检测信息添加到epoll实例中
    epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    epoll_event epevs[1024]{};
    while (true) {
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if (ret == -1) {
            std::cerr << "epoll_wait" << std::endl;
            exit(-1);
        }
        std::cout << "ret = " << ret << std::endl;
        for (int i = 0; i < ret; i++) {
            int curfd = epevs[i].data.fd;
            if (curfd == lfd) {
                //监听的文件描述符有数据到达，有客服端连接
                sockaddr_in cliaddr;
                socklen_t len = sizeof(cliaddr);
                int cfd = accept(lfd, (sockaddr *)&cliaddr, &len);
                epev.events = EPOLLIN;
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            } else {
                //有数据达到，需要通信
                char buf[5]{};
                int len = read(curfd, buf, sizeof(buf));
                if (len == -1) {
                    std::cerr << "read" << std::endl;
                    exit(-1);
                } else if (!len) {
                    std::cout << "client closed...\n";
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    close(curfd);
                } else if (len > 0) {
                    std::cout << "read buf: " << buf << std::endl;
                    write(curfd, buf, strlen(buf) + 1);
                }
            }
        }
    }

    close(lfd);
    close(epfd);

    return 0;
}