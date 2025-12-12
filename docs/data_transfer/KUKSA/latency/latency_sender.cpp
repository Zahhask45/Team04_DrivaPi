#include <iostream>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>
#include <cstring>
#include <random>
#include <chrono>


const char* INTERFACE = "vcan0";
const int NUM_SAMPLES = 1000;
const int DELAY_MS = 10;

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

    std::random_device rd; // Hardware random number generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_real_distribution<float> dis(0.0, 100.0); // Random speeds between 0.0 and 100.0

    std::cout << std::fixed <<std::setprecision(6);
    for (int i = 0; i < NUM_SAMPLES; ++i)
    {
        struct can_frame frame;
        frame.can_id = 0x100; // CAN ID for speed data
        frame.can_dlc = sizeof(float); // Data length code

        float speed = dis(gen);

        std::memcpy(frame.data, &speed, sizeof(float));

        auto now = std::chrono::system_clock::now();
    }
    return 0;
}