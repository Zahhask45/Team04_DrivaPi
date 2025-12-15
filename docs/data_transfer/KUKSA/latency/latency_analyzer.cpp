#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <deque>
#include <cmath>
#include <algorithm>

// Data structure: Map a KEY (int) to a QUEUE of timestamps
// Example: Speed 50.123 -> [Time_Sent_1, Time_Sent_2, ...]
std::map<int, std::deque<double>> sent_timestamps;
std::map<int, std::deque<float>> sent_speeds; // Keep original value for debugging

// Structure for final results
struct Result {
    float speed;
    double t_sent;
    double t_recv;
    double latency_ms;
};
std::vector<Result> results;

// Global variable to store CAN timestamp (since it appears on the line before the value)
double g_lastCanTimestamp = 0.0;

// Helper: Convert float to integer key (3 decimal places precision to match CAN logs)
int get_key(float speed) {
    return std::round(speed * 1000.0f);
}

// Helper: Find a number in a string following a specific "keyword"
bool extractValue(const std::string& line, const std::string& keyword, double& value) {
    size_t pos = line.find(keyword);
    if (pos == std::string::npos) return false;
    
    // Try to read the number immediately after the keyword length
    try {
        std::string remainder = line.substr(pos + keyword.length());
        value = std::stod(remainder);
        return true;
    } catch (...) {
        return false;
    }
}

// Sender Log Parser
bool parseSender(const std::string& line, float& speed, double& t0) {
    // Format: "Sent speed: 49.459042 at timestamp: 1765553122.791886"
    double spd_dbl;
    if (extractValue(line, "speed: ", spd_dbl) && extractValue(line, "timestamp: ", t0)) {
        speed = (float)spd_dbl;
        return true;
    }
    return false;
}

// Receiver Log Parser (Handles both KUKSA and CAN formats)
bool parseReceiver(const std::string& line, float& speed, double& t1) {
    double temp_val;

    // --- KUKSA CASE ---
    // Format: "KuksaReader: Received speed: 89.7408 at 1765553552797"
    if (line.find("KuksaReader:") != std::string::npos) {
        if (extractValue(line, "speed: ", temp_val)) {
            speed = (float)temp_val;
            if (extractValue(line, "at ", t1)) {
                t1 /= 1000.0; // convert ms to seconds
                return true;
            }
        }
    }

    // --- CAN CASE (Split into two lines) ---
    // Line 1: "CANReader: ... at 1765553122792"
    if (line.find("CANReader:") != std::string::npos && extractValue(line, "at ", temp_val)) {
        g_lastCanTimestamp = temp_val;
        return false; // We only caught the timestamp, waiting for value
    }
    // Line 2: "Speed set to (m/s): 49.459"
    if (line.find("Speed set to") != std::string::npos) {
        if (extractValue(line, ": ", temp_val)) {
            speed = (float)temp_val;
            t1 = g_lastCanTimestamp / 1000.0; // Use the timestamp from the previous line
            return true;
        }
    }

    return false;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: ./latency_analyzer <sender.txt> <receiver.txt>" << std::endl;
        return 1;
    }

    std::string senderFile = argv[1];
    std::string receiverFile = argv[2];
    std::string outputFile = receiverFile + "_analise.csv";

    // 1. READ SENDER (Populate Queue)
    std::ifstream fSend(senderFile);
    int countSent = 0;
    std::string line;
    while (std::getline(fSend, line)) {
        float spd; double t0;
        if (parseSender(line, spd, t0)) {
            int key = get_key(spd);
            sent_timestamps[key].push_back(t0); // Add to end of queue
            sent_speeds[key].push_back(spd);
            countSent++;
        }
    }
    fSend.close();
    std::cout << "Sender: Loaded " << countSent << " samples." << std::endl;

    // 2. READ RECEIVER (Consume Queue)
    std::ifstream fRecv(receiverFile);
    int countRecv = 0;
    while (std::getline(fRecv, line)) {
        float spd; double t1;
        if (parseReceiver(line, spd, t1)) {
            int key = get_key(spd);
            
            // Check if this speed exists in our map and has pending timestamps
            if (sent_timestamps[key].size() > 0) {
                // FIFO: Get the oldest send time
                double t0 = sent_timestamps[key].front();
                sent_timestamps[key].pop_front(); 
                
                double lat = (t1 - t0) * 1000.0;
                
                // Sanity filter (-500ms to 5000ms) to ignore clock sync errors
                if (lat > -500 && lat < 5000) {
                    results.push_back({spd, t0, t1, lat});
                    countRecv++;
                }
            }
        }
    }
    fRecv.close();
    std::cout << "Receiver: Correlated " << countRecv << " samples." << std::endl;

    // 3. STATISTICS
    if (results.empty()) {
        std::cerr << "ERROR: No matches found. Check the log files." << std::endl;
        return 1;
    }

    double sum = 0, minLat = 99999, maxLat = -99999;
    std::ofstream fOut(outputFile);
    fOut << "Speed,T_Sent,T_Recv,Latency_ms\n";

    for (const auto& r : results) {
        sum += r.latency_ms;
        if (r.latency_ms < minLat) minLat = r.latency_ms;
        if (r.latency_ms > maxLat) maxLat = r.latency_ms;
        fOut << r.speed << "," << std::fixed << std::setprecision(6) 
             << r.t_sent << "," << r.t_recv << "," << std::setprecision(3) << r.latency_ms << "\n";
    }
    fOut.close();

    double avg = sum / results.size();
    double sqSum = 0;
    for (const auto& r : results) sqSum += std::pow(r.latency_ms - avg, 2);
    double stdDev = std::sqrt(sqSum / results.size());
    double loss = 100.0 * (1.0 - (double)countRecv / (double)countSent);

    std::cout << "\n=== PERFORMANCE REPORT ===" << std::endl;
    std::cout << "Sent:           " << countSent << std::endl;
    std::cout << "Received:       " << countRecv << std::endl;
    std::cout << "Packet Loss:    " << std::fixed << std::setprecision(2) << loss << " %" << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Avg Latency:    " << std::setprecision(3) << avg << " ms" << std::endl;
    std::cout << "Min Latency:    " << minLat << " ms" << std::endl;
    std::cout << "Max Latency:    " << maxLat << " ms" << std::endl;
    std::cout << "Jitter (StdDev):" << stdDev << " ms" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "CSV file generated: " << outputFile << std::endl;

    return 0;
}