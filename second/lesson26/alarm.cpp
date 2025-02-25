#include <unistd.h>
#include <iostream>

int main() {
    int seconds = alarm(5);
    std::cout << "seconds: " << seconds << std::endl; //0
    sleep(2);
    seconds = alarm(10); // 不阻塞
    std::cout << "seconds: " << seconds << std::endl; //3
    while (true) {}

    return 0;
}