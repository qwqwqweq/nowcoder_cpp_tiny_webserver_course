#include <iostream>
#include <bit>

int main() {

    union {
        short value;
        char bytes[sizeof(short)];
    } test;
    
    test.value = 0x0102;
    if (test.bytes[0] == 1 && test.bytes[1] == 2)
        std::cout << "big\n";
    else if (test.bytes[0] == 2 && test.bytes[1] == 1)
        std::cout << "small\n";
    else 
        std::cout << "other\n";

    if constexpr (std::endian::native == std::endian::little)
        std::cout << "small\n";
    else if constexpr (std::endian::native == std::endian::big)
        std::cout << "big\n";
    else 
        std::cout << "other\n";

    return 0;
}