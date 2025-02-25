#include <iostream>
#include <thread>

int main() {
    int num = 10;
    std::thread t1([&]() {
        std::cout << "child thread...\n";
        std::cout << "arg value: " << num << std::endl; 
    });
    
    for (int i = 0; i < 5; i++)
        std::cout << i << std::endl;

    t1.join();

    return 0;
}