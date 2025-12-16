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
double g_lastCanTimestamp = 0.0;

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
    // KUKSA
    if (line.find("Received speed:") != std::string::npos)
    {
        if (extractValue(line, "speed: ", temp_val))
        {
            speed = (float)temp_val;
            if (extractValue(line, "at ", t1))
            {
                t1 /= 1000.0; 
                return true;
            }
        }
    }
    // CAN
    if (line.find("at ") != std::string::npos && line.find("Received speed:") == std::string::npos)
    {
        if (extractValue(line, "at ", temp_val)) { g_lastCanTimestamp = temp_val; return false; }
    }
    if (line.find("Speed set to") != std::string::npos)
    {
        if (extractValue(line, ": ", temp_val))
        {
            speed = (float)temp_val;
            if (g_lastCanTimestamp > 0.0)
            { 
                t1 = g_lastCanTimestamp / 1000.0;
                return true;
            }
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

    std::cout << "Method: Nearest Time Match (Permissive)..." << std::endl;

    // 1. CARREGAR SENDER
    std::ifstream fSend(argv[1]);
    if (!fSend.is_open())
    {
        return 1;
    }
    std::string line;
    while (std::getline(fSend, line))
    {
        processSenderLine(line);
    }
    fSend.close();
    std::cout << "[Sender] Loaded: " << sender_list.size() << std::endl;

    // 2. PROCESSAR RECEIVER
    std::ifstream fRecv(argv[2]);
    int countRecvLines = 0;

    while (std::getline(fRecv, line))
    {
        float r_speed; 
        double r_time;
        
        if (parseReceiverLine(line, r_speed, r_time))
        {
            countRecvLines++;
            
            // BUSCA UNIVERSAL
            int bestIndex = -1;
            double minAbsTimeDiff = 100000.0; 

            for (size_t i = 0; i < sender_list.size(); i++)
            {
                if (sender_list[i].matched) continue;

                // Tolerância aumentada para 0.1 para garantir que apanha tudo
                if (std::abs(sender_list[i].speed - r_speed) < 0.1f)
                {
                    
                    double diff = std::abs(r_time - sender_list[i].timestamp);
                    
                    if (diff < minAbsTimeDiff)
                    {
                        minAbsTimeDiff = diff;
                        bestIndex = i;
                    }
                }
            }

            if (bestIndex != -1)
            {
                if (minAbsTimeDiff < 10.0) 
                { 
                    sender_list[bestIndex].matched = true;
                    double lat = (r_time - sender_list[bestIndex].timestamp) * 1000.0;
                    results.push_back({r_speed, sender_list[bestIndex].timestamp, r_time, lat});
                }
            }
        }
    }
    fRecv.close();

    // 3. ESTATÍSTICAS
    std::string outName = std::string(argv[2]) + "_analise.csv";
    std::ofstream fOut(outName);
    fOut << "Speed,TS_Sent,TS_Recv,Latency_ms\n";

    std::vector<double> stats_lats;
    for (const auto& r : results)
    {
        fOut << r.speed << "," << std::fixed << std::setprecision(6) 
             << r.t_sent << "," << r.t_recv << "," 
             << std::setprecision(3) << r.latency_ms << "\n";
        
        // Estatísticas apenas em valores razoáveis
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