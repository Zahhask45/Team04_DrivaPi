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
    
    std::strcpy(ifr.ifr_name, INTERFACE);
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0)
    {
        std::cerr << "Error getting interface index" << std::endl;
        return 1;
    }

    std::memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Error in socket bind" << std::endl;
        return 1;
    }
    
    return 0;
}