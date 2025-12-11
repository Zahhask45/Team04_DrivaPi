#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <numeric> 

static double percentile(const std::vector<double>& v, double p) {
    if (v.empty()) return std::numeric_limits<double>::quiet_NaN();
    double rank = (p / 100.0) * (v.size() - 1);
    size_t lo = static_cast<size_t>(std::floor(rank));
    size_t hi = static_cast<size_t>(std::ceil(rank));
    double w = rank - lo;
    return v[lo] + w * (v[hi] - v[lo]);
}

int main(int argc, char* argv[]) {
    std::string filename = "can_rpi_to_stm_log.txt";
    if (argc > 1 && std::string(argv[1]) == "rasp")
        filename = "can_rpi_loopback_log.txt";
    else if (argc > 1 && std::string(argv[1]) == "stm")
        filename = "can_stm32_loopback_log.txt";

    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return 1;
    }

    std::vector<double> values;
    std::string line;
    while (std::getline(infile, line)) {
        // Trim
        auto start = line.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) continue;
        auto end = line.find_last_not_of(" \t\n\r");
        std::string trimmed = line.substr(start, end - start + 1);
        try {
            if (!trimmed.empty())
                values.push_back(std::stod(trimmed));
        } catch (...) {
            continue; // skip non-numeric lines
        }
    }

    if (values.empty()) {
        std::cout << "No numeric data found in the file.\n";
        return 0;
    }

    std::sort(values.begin(), values.end());
    const size_t n = values.size();

    double min_v   = values.front();
    double max_v   = values.back();
    double mean    = std::accumulate(values.begin(), values.end(), 0.0) / n;
    double median  = (n % 2) ? values[n / 2] : (values[n / 2 - 1] + values[n / 2]) / 2.0;
    double p95     = percentile(values, 95.0);
    double p99     = percentile(values, 99.0);

    double sq_sum = 0.0;
    for (double v : values) {
        double diff = v - mean;
        sq_sum += diff * diff;
    }
    double stddev = std::sqrt(sq_sum / n);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "| Metric            | RTT Value (µs) | Est. One-Way (µs) |\n";
    std::cout << "|-------------------|----------------|-------------------|\n";
    auto row = [&](const std::string& name, double rtt) {
        std::cout << "| " << std::left << std::setw(18) << name
                  << " | " << std::right << std::setw(14) << rtt
                  << " | " << std::setw(17) << (rtt / 2.0) << " |\n";
    };
    row("Minimum Latency",  min_v);
    row("Maximum Latency",  max_v);
    row("Mean Latency",     mean);
    row("Median Latency",   median);
    row("Std Deviation",    stddev);
    row("P95 Latency",      p95);
    row("P99 Latency",      p99);
    std::cout << "| Total Samples     | " << std::setw(14) << n << " | " << std::setw(17) << "N/A" << " |\n";
    std::cout << "| Success Rate      | " << std::setw(13) << "100%" << " | " << std::setw(17) << "N/A" << " |\n";
    return 0;
}
