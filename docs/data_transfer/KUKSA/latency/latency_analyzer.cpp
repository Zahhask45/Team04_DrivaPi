#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

// Define a structure to hold latency sample data
struct Sample
{
    float speed;
    double t0; // timestamp when sent
    double t1; // timestamp when received
    bool received;
}

// Auxiliary function to parse line from Sender log
Sample parseSenderLogLine(const std::string &line, float &speed, double& timestamp)
{
    std::istringstream iss(line);
    std::string temp;
    
    iss >> temp >> temp; // Skip "Sender:" and "Sent"
    if (iss.fail()) return false;
    iss >> speed;
    iss >> temp >> temp; // Skip "at" "timestamp:"

    iss >> timestamp;
    return (!iss.fail());
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: latency_analyzer <sender_log_file> <receiver_log_file>" << std::endl;
        return 1;
    }

    std::string senderFile = argv[1];
    std::string receiverFile = argv[2];
    
    std::cout << "Latency Analyzer" << std::endl;
    return 0;
}