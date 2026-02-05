#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "Board.h"
#include "ParseUtils.h"

using namespace sudoku;

namespace {

void printUsage(const char* program)
{
    std::cout << "Usage: " << program << " [--iterations N]\n";
    std::cout << "  --iterations N  Number of runs (default 100)\n";
}

} // namespace

int main(int argc, const char* argv[])
{
    unsigned int iterations = 100;

    for(int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if(arg == "--help" || arg == "-h")
        {
            printUsage(argv[0]);
            return 0;
        }
        if(arg == "--iterations" && i + 1 < argc)
        {
            unsigned int parsed = 0;
            if(!parseUnsigned(argv[++i], parsed))
            {
                std::cerr << "Invalid value for --iterations\n";
                return 1;
            }
            iterations = parsed;
            continue;
        }

        std::cerr << "Unknown argument: " << arg << "\n";
        printUsage(argv[0]);
        return 1;
    }

    std::vector<double> timesMs;
    timesMs.reserve(iterations);

    for(unsigned int i = 0; i < iterations; ++i)
    {
        Board board;
        auto start = std::chrono::steady_clock::now();
        board.generateSolution();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        timesMs.push_back(elapsed.count());
    }

    double total = 0.0;
    double min = timesMs.empty() ? 0.0 : timesMs.front();
    double max = timesMs.empty() ? 0.0 : timesMs.front();
    for(double value : timesMs)
    {
        total += value;
        if(value < min)
            min = value;
        if(value > max)
            max = value;
    }

    double avg = iterations == 0 ? 0.0 : total / static_cast<double>(iterations);

    std::cout << "Sudoku Generation Benchmark\n";
    std::cout << "Runs: " << iterations << "\n\n";
    std::cout << std::left << std::setw(12) << "Metric" << std::right << std::setw(12) << "Milliseconds" << "\n";
    std::cout << std::string(24, '-') << "\n";
    std::cout << std::left << std::setw(12) << "Avg" << std::right << std::setw(12) << std::fixed << std::setprecision(3) << avg << "\n";
    std::cout << std::left << std::setw(12) << "Min" << std::right << std::setw(12) << std::fixed << std::setprecision(3) << min << "\n";
    std::cout << std::left << std::setw(12) << "Max" << std::right << std::setw(12) << std::fixed << std::setprecision(3) << max << "\n";

    return 0;
}
