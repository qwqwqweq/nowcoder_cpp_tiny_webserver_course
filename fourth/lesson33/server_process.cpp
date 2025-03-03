#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>
#include <wait.h>
#include <errno.h>

void recyleChild(int arg) {
    while (true) {
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret == -1) {
            //所有子进程回收完成
            break;
        } else if (ret == 0) {
            //还有子进程活着
            break;
        } else if (ret > 0) {
            //被回收了
            std::cout << "子进程被回收了：" << ret << std::endl;
        }
    }
}

int main() {

    //注册信号捕捉
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recyleChild;
    sigaction(SIGCHLD, &act, NULL);

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
    
    //不断循环等待客户端连接
    while (true) {
        //接受连接
        sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int cfd = accept(lfd, (sockaddr *)&cliaddr, &len);
        if (cfd == -1) {
            if (errno == EINTR) {
                continue;
            }
            std::cerr << "accept" << std::endl;
            exit(-1);
        }

        //每一个连接进来，创建一个子进程跟客户端通信
        pid_t pid = fork();
        if (!pid) {
            //子进程
            //获取客户端的信息
            char clientIP[16]{};
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
            unsigned short port = ntohs(cliaddr.sin_port);
            std::cout << "client ip is : " << clientIP << " port is : " << port << std::endl;

            //接受客户端发来的数据
            char recvBuf[1024]{};
            while (true) {
                int len = read(cfd, &recvBuf, sizeof(recvBuf));
                if (len == -1) {
                    std::cerr << "read" << std::endl;
                    exit(-1);
                } else if (len > 0) {
                    std::cout << "recv client data : " << recvBuf << std::endl;
                } else {
                    std::cout << "client closed..." << std::endl;
                    break;
                }
                write(cfd, recvBuf, strlen(recvBuf));
            }
            close(cfd);
            exit(0); //退出当前子进程
        }
    }

    close(lfd);

    return 0;
}