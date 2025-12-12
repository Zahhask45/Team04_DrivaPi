#include <iostream>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>


const char* INTERFACE = "vcan0";
int main() {
    std::cout << "Starting latency sender on interface: " << INTERFACE << std::endl;
    std::cout << "Using floats (4 bytes) for data transfer." << std::endl;

    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;

    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        std::cerr << "Error while opening socket" << std::endl;
        return 1;
    }
    
    return 0;
}