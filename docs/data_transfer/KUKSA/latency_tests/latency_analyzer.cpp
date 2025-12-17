#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>

struct SentPacket
{
    float speed;
    double timestamp;
    bool matched; 
};

struct Result
{
    float speed;
    double t_sent;
    double t_recv;
    double latency_ms;
};

std::vector<SentPacket> sender_list;
std::vector<Result> results;

// --- HELPER ---
bool extractValue(const std::string& line, const std::string& keyword, double& value)
{
    size_t pos = line.find(keyword);
    if (pos == std::string::npos)
    {
        return false;
    }
    try
    {
        std::string remainder = line.substr(pos + keyword.length());
        value = std::stod(remainder);
        return true;
    } catch (...)
    {
        return false;
    }
}

// --- PARSERS ---
void processSenderLine(const std::string& line)
{
    double spd_dbl, t0;
    if (extractValue(line, "speed: ", spd_dbl) && extractValue(line, "timestamp: ", t0))
    {
        sender_list.push_back({ (float)spd_dbl, t0, false });
    }
}

bool parseReceiverLine(const std::string& line, float& speed, double& t1)
{
    double temp_val;
    
    // Check for KUKSA format
    if (line.find("KuksaReader: Received speed:") != std::string::npos)
    {
        if (extractValue(line, "speed: ", temp_val))
        {
            speed = (float)temp_val;
            if (extractValue(line, "at ", t1)) return true;
        }
    }

    // Check for CAN format (UPDATED TO MATCH YOUR LOGS)
    // Log line: "CANReader: Received speed: 29.3048 at 39930.388230"
    if (line.find("CANReader: Received speed:") != std::string::npos)
    {
         if (extractValue(line, "speed: ", temp_val))
        {
            speed = (float)temp_val;
            // The timestamp is now directly in seconds (steady_clock), same as KUKSA
            if (extractValue(line, "at ", t1)) return true;
        }
    }

    return false;
}

// --- MAIN ---
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./latency_analyzer <sender.txt> <receiver.txt>" << std::endl;
        return 1;
    }

    std::cout << "Method: Nearest Time Match (Steady Clock Precision)..." << std::endl;

    // 1. LOAD SENDER LOGS
    std::ifstream fSend(argv[1]);
    if (!fSend.is_open())
    {
        std::cerr << "Error: Could not open sender file." << std::endl;
        return 1;
    }
    std::string line;
    while (std::getline(fSend, line))
    {
        processSenderLine(line);
    }
    fSend.close();
    std::cout << "[Sender] Loaded: " << sender_list.size() << " packets." << std::endl;

    // 2. LOAD RECEIVER LOGS
    std::ifstream fRecv(argv[2]);
    if (!fRecv.is_open())
    {
        std::cerr << "Error: Could not open receiver file." << std::endl;
        return 1;
    }
    int countRecvLines = 0;

    while (std::getline(fRecv, line))
    {
        float r_speed; 
        double r_time;
        
        if (parseReceiverLine(line, r_speed, r_time))
        {
            countRecvLines++;
            
            // "NEAREST MATCH" ALGORITHM
            int bestIndex = -1;
            // Search window (e.g., look for packets sent +/- 100ms around receive time)
            double minAbsTimeDiff = 100000.0; 

            for (size_t i = 0; i < sender_list.size(); i++)
            {
                if (sender_list[i].matched) continue;

                // 1. Check Data Integrity (Tolerance 0.1 for float rounding)
                if (std::abs(sender_list[i].speed - r_speed) < 0.1f)
                {
                    // 2. Check Time Correlation
                    // Note: r_time and timestamp are both in seconds now
                    double diff = std::abs(r_time - sender_list[i].timestamp);
                    
                    if (diff < minAbsTimeDiff)
                    {
                        minAbsTimeDiff = diff;
                        bestIndex = i;
                    }
                }
            }

            // Match Threshold: 
            // If the closest matching speed value was sent > 2.0 seconds away, it's likely a different packet (collision).
            if (bestIndex != -1)
            {
                if (minAbsTimeDiff < 2.0) 
                { 
                    sender_list[bestIndex].matched = true;
                    // Latency = (Recv - Sent) * 1000 to get ms
                    double lat = (r_time - sender_list[bestIndex].timestamp) * 1000.0;
                    results.push_back({r_speed, sender_list[bestIndex].timestamp, r_time, lat});
                }
            }
        }
    }
    fRecv.close();

    // 3. STATS & OUTPUT
    std::string outName = std::string(argv[2]) + "_analise.csv";
    std::ofstream fOut(outName);
    fOut << "Speed,TS_Sent,TS_Recv,Latency_ms\n";

    std::vector<double> stats_lats;
    for (const auto& r : results)
    {
        fOut << r.speed << "," << std::fixed << std::setprecision(6) 
             << r.t_sent << "," << r.t_recv << "," 
             << std::setprecision(3) << r.latency_ms << "\n";
        
        // Filter massive outliers for statistics (e.g. process paused for > 2 seconds)
        if (std::abs(r.latency_ms) < 2000.0)
        {
            stats_lats.push_back(r.latency_ms);
        }
    }
    fOut.close();

    double avg = 0, min = 0, max = 0, stdDev = 0;
    if (!stats_lats.empty())
    {
        double sum = 0;
        min = stats_lats[0]; max = stats_lats[0];
        for (double v : stats_lats)
        {
            sum += v;
            if (v < min) min = v;
            if (v > max) max = v;
        }
        avg = sum / stats_lats.size();
        
        double sqSum = 0;
        for (double v : stats_lats) 
        {
            sqSum += (v - avg) * (v - avg);
        }
        stdDev = std::sqrt(sqSum / stats_lats.size());
    }
    double loss = 100.0 * (1.0 - (double)results.size() / (double)sender_list.size());

    std::cout << "\n=== PERFORMANCE REPORT ===" << std::endl;
    std::cout << "Sender Lines:      " << sender_list.size() << std::endl;
    std::cout << "Receiver Parsed:   " << countRecvLines << std::endl;
    std::cout << "Receiver Matched:  " << results.size() << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Packet Loss:       " << std::fixed << std::setprecision(2) << loss << " %" << std::endl;
    std::cout << "Avg Latency:       " << std::setprecision(3) << avg << " ms" << std::endl;
    std::cout << "Min Latency:       " << min << " ms" << std::endl;
    std::cout << "Max Latency:       " << max << " ms" << std::endl;
    std::cout << "Jitter (Std):      " << stdDev << " ms" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "CSV file generated: " << outName << std::endl;

    return 0;
}