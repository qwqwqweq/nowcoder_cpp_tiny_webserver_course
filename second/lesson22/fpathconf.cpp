#include <unistd.h>
#include <iostream>
#include <format>
#include <cstring>

int main() {
    int pipefd[2];
    pipe(pipefd);

    long sz = fpathconf(pipefd[0], _PC_PIPE_BUF);
    std::cout << sz << '\n'; 
    std::cout << sizeof(long) << ' ' << sizeof(int) << '\n';
}