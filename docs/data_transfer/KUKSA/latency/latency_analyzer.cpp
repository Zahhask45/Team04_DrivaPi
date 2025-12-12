#include <iostream>

// Define a structure to hold latency sample data
struct Sample
{
    float speed;
    double t0; // timestamp when sent
    double t1; // timestamp when received
    bool received;
}

int main()
{
    std::cout << "Latency Analyzer" << std::endl;
    return 0;
}