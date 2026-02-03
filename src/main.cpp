//
//  main.cpp
//  Sudoku
//
//  Created by Marcin on 23/10/2022.
//

#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include "Board.h"

using namespace sudoku;

namespace {

void printUsage(const char* program)
{
    std::cout << "Usage: " << program << " [--clues N] [--seed N] [--solution]\n";
    std::cout << "  --clues N     Number of given cells (default 30)\n";
    std::cout << "  --seed N      Deterministic seed for generation\n";
    std::cout << "  --solution    Print a full solved board\n";
}

bool parseUnsigned(const std::string& value, unsigned int& out)
{
    if(value.empty())
        return false;
    char* end = nullptr;
    unsigned long parsed = std::strtoul(value.c_str(), &end, 10);
    if(end == value.c_str() || *end != '\0')
        return false;
    out = static_cast<unsigned int>(parsed);
    return true;
}

} // namespace

int main(int argc, const char * argv[]) {
    unsigned int clues = 30;
    bool solutionOnly = false;
    std::optional<uint32_t> seed;

    for(int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if(arg == "--help" || arg == "-h")
        {
            printUsage(argv[0]);
            return 0;
        }
        if(arg == "--solution")
        {
            solutionOnly = true;
            continue;
        }
        if(arg == "--clues" && i + 1 < argc)
        {
            unsigned int parsed = 0;
            if(!parseUnsigned(argv[++i], parsed))
            {
                std::cerr << "Invalid value for --clues\n";
                return 1;
            }
            clues = parsed;
            continue;
        }
        if(arg == "--seed" && i + 1 < argc)
        {
            unsigned int parsed = 0;
            if(!parseUnsigned(argv[++i], parsed))
            {
                std::cerr << "Invalid value for --seed\n";
                return 1;
            }
            seed = parsed;
            continue;
        }

        std::cerr << "Unknown argument: " << arg << "\n";
        printUsage(argv[0]);
        return 1;
    }

    std::unique_ptr<Board> board = std::make_unique<Board>(seed);

    if(solutionOnly)
    {
        while(!board->generateSolution());
    }
    else
    {
        while(!board->generatePuzzle(clues));
    }

    std::cout << *board;

    return 0;
}
