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

    std::string outputFile = receiverFile;
    size_t lastdot = outputFile.find_last_of(".");
    if (lastdot != std::string::npos)
    {
        outputFile = outputFile.substr(0, lastdot);
    }
    outputFile += "_analyzed.csv";

    std::cout << "Analyzing latency from sender log: " << senderFile << " and receiver log: " << receiverFile << std::endl;
    std::cout << "Output will be saved to: " << outputFile << std::endl;

    std::map<int, Sample> database;

    // Read sender log
    std::ifstream senderLog(senderFile);
    if (!senderLog.is_open())
    {
        std::cerr << "Error opening sender log file: " << senderFile << std::endl;
        return 1;
    }

    std::string line;
    int sentCount = 0;
    while (std::getline(senderLog, line))
    {
        float speed;
        double t0;
        if (parseSenderLogLine(line, speed, t0))
        {
            Sample sample;
            sample.speed = speed;
            sample.t0 = t0;
            sample.received = false;

            int key = std::round(speed * 1000); // Use speed in mm/s as key
            database[key] = sample;
            sentCount++;
        }
    }
    senderLog.close();

    std::cout << "Total samples sent: " << sentCount << std::endl;

    
    return 0;
}