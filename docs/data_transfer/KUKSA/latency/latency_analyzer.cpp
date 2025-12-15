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

    // Read receiver log
    std::ifstream receiverLog(receiverFile);
    if (!receiverLog.is_open())
    {
        std::cerr << "Error opening receiver log file: " << receiverFile << std::endl;
        return 1;
    }
    int receivedCount = 0;
    while (std::getline(receiverLog, line))
    {
        float speed;
        double t1;
        if (parseSenderLogLine(line, speed, t1))
        {
            int key = std::round(speed * 1000);
            auto it = database.find(key);
            if (it != database.end())
            {
                it->second.t1 = t1;
                it->second.received = true;
                receivedCount++;
            }
        }
    }
    receiverLog.close();

    std::cout << "Total samples received: " << receivedCount << std::endl;

    // Write analysis to output CSV
    std::ofstream outFile(outputFile);
    std:vector<double> latencies;

    outFile << "Speed (m/s),Sent Timestamp (s),Received Timestamp (s),Latency (ms)" << std::endl;
    for (const auto &[key, sample] : database)
    {
        if (sample.received)
        {
            double latency = (sample.t1 - sample.t0) * 1000.0; // Convert to ms
            if (latency > -1000 && latency < 10000) // Filter out unreasonable latencies
            {
                latencies.push_back(latency);
                outFile << std::fixed << std::setprecision(3)
                        << sample.speed << ","
                        << sample.t0 << ","
                        << sample.t1 << ","
                        << latency << std::endl;
            }
        }
    }
    outFile.close();

    if (latencies.empty())
    {
        std::cerr << "No valid latency samples to analyze." << std::endl;
    }
    else
    {
        double sum = 0.0;
        double minLatency = latencies[0];
        double maxLatency = latencies[0];
        for (double latency : latencies)
        {
            sum += latency;
            if (latency < minLatency) minLatency = latency;
            if (latency > maxLatency) maxLatency = latency;
        }
        double avgLatency = sum / latencies.size();

        double sqrSum = 0.0;
        for (double latency : latencies)
        {
            sqrSum += std::pow(latency - avgLatency, 2);
        }
        double stddevLatency = std::sqrt(sqrSum / latencies.size());

        double lossRate = 0;
        if (sentCount > 0)
        {
            lossRate = 100.0 * (1.0 - static_cast<double>(receivedCount) / static_cast<double>(sentCount));
        }
        std::cout << "\n=== PERFORMANCE REPORT ===" << std::endl;
        std::cout << "Total Samples Sent: " << sentCount << std::endl;
        std::cout << "Total Samples Received: " << receivedCount << std::endl;
        std::cout << "Packet lost: "<< std::fixed << std::setprecision(2)<< lossRate << " %" << std::endl;
        std::cout << "==========================" << std::endl;
        std::cout << "Average Latency: " << avgLatency << " ms" << std::endl;
        std::cout << "Minimum Latency: " << minLatency << " ms" << std::endl;
        std::cout << "Maximum Latency: " << maxLatency << " ms" << std::endl;
        std::cout << "Standard Deviation: " << stddevLatency << " ms" << std::endl;
    }
    std::cout << "Analysis complete. Results saved to " << outputFile << std::endl;

    return 0;
}