#include <arpa/inet.h>
#include <iostream>

int main() {
    char buf[] = "192.168.1.2";
    //点分十进制ip字符串转换成网络字节序的整数
    unsigned int num = 0;
    inet_pton(AF_INET, buf, &num);
    unsigned char *p = (unsigned char *)&num;
    std::cout << (int)*p << '.' << (int)*(p + 1) << '.' << (int)*(p + 2) << '.' << (int)*(p + 3) << std::endl;

    //将网络字节序的ip整数转换成点分十进制的ip字符串
    char ip[16]{};
    const char *str = inet_ntop(AF_INET, &num, ip, 16);
    std::cout << str << ' ' << ip << std::endl;

    return 0;
}