#include <iostream>

const char* INTERFACE = "vcan0";
int main() {
    std::cout << "Starting latency sender on interface: " << INTERFACE << std::endl;
    std::cout << "Using floats (4 bytes) for data transfer." << std::endl;

    
    return 0;
}