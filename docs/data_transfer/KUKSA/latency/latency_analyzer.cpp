#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <cmath>
#include <algorithm>

// Define a structure to hold latency sample data
struct Sample
{
    float speed;
    double t0; // timestamp when sent
    double t1; // timestamp when received
    bool received;
}; // <--- CORREÇÃO: Faltava o ponto e vírgula aqui

// Variável global para guardar o timestamp do CAN (pois aparece na linha anterior ao valor)
double g_lastCanTimestamp = 0.0;

// Function to parse a line from the receiver log (Handles both KUKSA and CAN formats)
bool parseReceiverLogLine(const std::string &line, float &speed, double& timestamp)
{
    std::stringstream iss(line);
    std::string temp;

    // --- CASO 1: KUKSA ---
    // Formato: "KuksaReader: Received speed: 89.7408 at 1765553552797"
    if (line.find("Received speed:") != std::string::npos)
    {
        // Ignorar tudo até encontrar "speed:"
        while (iss >> temp && temp != "speed:") {} 
        
        if (iss.fail()) return false;
        iss >> speed;
        iss >> temp; // Skip "at"
        iss >> timestamp;
        timestamp /= 1000.0; // Convert ms to s 
        return true;
    }

    // --- CASO 2: CAN (Parte 1 - Ler Timestamp) ---
    // Formato: "CANReader: Received CAN frame ... at 1765553122792"
    if (line.find("CANReader: Received CAN frame") != std::string::npos && line.find("at") != std::string::npos)
    {
        // Encontrar a posição de "at" e ler o número a seguir
        size_t pos = line.rfind("at");
        std::string tsStr = line.substr(pos + 3);
        try {
            g_lastCanTimestamp = std::stod(tsStr);
        } catch(...) {}
        return false; // Ainda não temos a velocidade, só guardamos o tempo
    }

    // --- CASO 2: CAN (Parte 2 - Ler Velocidade) ---
    // Formato: "Speed set to (m/s): 49.459"
    if (line.find("Speed set to") != std::string::npos)
    {
        // Ignorar tudo até encontrar "(m/s):"
        while (iss >> temp && temp != "(m/s):") {}
        if (iss.fail()) return false;

        iss >> speed;
        // Usar o timestamp guardado na linha anterior
        timestamp = g_lastCanTimestamp / 1000.0; 
        return true;
    }

    return false;
}

// Function to parse line from Sender log
bool parseSenderLogLine(const std::string &line, float &speed, double& timestamp)
{
    std::stringstream iss(line);
    std::string temp;
    
    // Formato: "Sent speed: 49.459... at timestamp: 12345..."
    if (line.find("Sent speed:") != std::string::npos) {
        iss >> temp >> temp; // Skip "Sent" "speed:"
        if (iss.fail()) return false;
        iss >> speed;
        iss >> temp >> temp; // Skip "at" "timestamp:"
        iss >> timestamp;
        return true;
    }
    return false;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./latency_analyzer <sender_log_file> <receiver_log_file>" << std::endl;
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

    // 1. Read sender log
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
            sample.t1 = 0.0;
            sample.received = false;

            // Usar arredondamento x100 para tolerar pequenas diferenças de float
            int key = std::round(speed * 100); 
            database[key] = sample;
            sentCount++;
        }
    }
    senderLog.close();
    std::cout << "Total samples sent: " << sentCount << std::endl;

    // 2. Read receiver log
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
        // CORREÇÃO: Usar a função de parse do Receiver, não do Sender
        if (parseReceiverLogLine(line, speed, t1))
        {
            int key = std::round(speed * 100);
            auto it = database.find(key);
            if (it != database.end())
            {
                // Só processa se ainda não foi recebido (evita duplicados)
                if (!it->second.received) {
                    it->second.t1 = t1;
                    it->second.received = true;
                    receivedCount++;
                }
            }
        }
    }
    receiverLog.close();

    std::cout << "Total samples received: " << receivedCount << std::endl;

    // 3. Write analysis to output CSV
    std::ofstream outFile(outputFile);
    std::vector<double> latencies; // CORREÇÃO: std::vector

    outFile << "Speed (m/s),Sent Timestamp (s),Received Timestamp (s),Latency (ms)" << std::endl;
    for (const auto &[key, sample] : database)
    {
        if (sample.received)
        {
            double latency = (sample.t1 - sample.t0) * 1000.0; // Convert to ms
            
            // Filtro para remover valores absurdos (ex: dessincronia de relógio)
            if (latency > -1000 && latency < 10000) 
            {
                latencies.push_back(latency);
                outFile << std::fixed << std::setprecision(3)
                        << sample.speed << ","
                        << std::setprecision(6) << sample.t0 << ","
                        << sample.t1 << ","
                        << std::setprecision(3) << latency << std::endl;
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
        std::cout << "Average Latency: " << std::fixed << std::setprecision(3) << avgLatency << " ms" << std::endl;
        std::cout << "Minimum Latency: " << minLatency << " ms" << std::endl;
        std::cout << "Maximum Latency: " << maxLatency << " ms" << std::endl;
        std::cout << "Standard Deviation: " << stddevLatency << " ms" << std::endl;
    }
    std::cout << "Analysis complete. Results saved to " << outputFile << std::endl;

    return 0;
}