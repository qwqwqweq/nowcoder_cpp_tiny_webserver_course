#include <iostream>
#include <arpa/inet.h>

int main() {

    //htons转换端口 
    unsigned short a = 0x0102;
    std::cout << std::hex << a << std::endl;
    unsigned short b = htons(a);
    std::cout << std::hex << b << std::endl;

    //htonl 转换ip
    unsigned char buf[4] = {192, 168, 1, 1};
    int num = *(int *)buf;
    int sum = htonl(num);
    unsigned char *p = (unsigned char *)&sum;
    std::cout << std::dec << (int)*p << ' ' << (int)*(p + 1) << ' ' << (int)*(p + 2) << ' ' << (int)*(p + 3) << '\n';

    //ntohl
    unsigned char buf1[4] = {1, 1, 168, 192};
    int num1 = *(int *)buf1;
    int sum1 = ntohl(num1);
    unsigned char *p1 = (unsigned char *)&sum1;
    std::cout << std::dec << (int)*p1 << ' ' << (int)*(p1 + 1) << ' ' << (int)*(p1 + 2) << ' ' << (int)*(p1 + 3) << '\n';

    //ntohs
    unsigned short c = 0x0102;
    std::cout << std::hex << c << std::endl;
    unsigned short d = ntohs(c);
    std::cout << std::hex << d << std::endl;

    return 0;
}