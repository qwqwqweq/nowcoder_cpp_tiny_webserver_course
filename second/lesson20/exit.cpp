#include <iostream>
#include <unistd.h>

int main() {
    std::cout << "Hello\n";
    std::cout << "World";

    // exit(0);
    _exit(0);

    return 0;
}