//
//  Board.cpp
//  Sudoku
//
//  Created by Marcin on 23/10/2022.
//

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include "Board.h"

using namespace sudoku;

Board::Board(std::optional<uint32_t> seed)
    : _boardData(std::make_unique<std::vector<unsigned int>>(BOARD_DIMENSION * BOARD_DIMENSION)),
      _rng(seed.has_value() ? seed.value() : std::random_device{}())
{
}

Board::~Board()
{
}

void Board::setSeed(uint32_t seed)
{
    _rng.seed(seed);
}

bool Board::generateSolution()
{
    std::fill(_boardData->begin(), _boardData->end(), 0);
    return solveBoard(*_boardData, true);
}

bool Board::generatePuzzle(unsigned int clues)
{
    if(clues > BOARD_DIMENSION * BOARD_DIMENSION)
        return false;

    if(!generateSolution())
        return false;

    std::vector<unsigned int> indices(BOARD_DIMENSION * BOARD_DIMENSION);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), _rng);

    unsigned int remaining = BOARD_DIMENSION * BOARD_DIMENSION;
    for(unsigned int index : indices)
    {
        if(remaining <= clues)
            break;

        unsigned int previous = _boardData->at(index);
        _boardData->at(index) = 0;

        if(countSolutions(2) == 1)
        {
            --remaining;
        }
        else
        {
            _boardData->at(index) = previous;
        }
    }

    return remaining == clues;
}

unsigned int Board::countSolutions(unsigned int limit) const
{
    std::vector<unsigned int> data = *_boardData;
    return countSolutionsImpl(data, limit);
}

unsigned int Board::getIndexForRowColumn(unsigned int row, unsigned int column) const
{
    return row * BOARD_DIMENSION + column;
}

uint16_t Board::getUsedMaskForCell(const std::vector<unsigned int>& data, unsigned int row, unsigned int column) const
{
    uint16_t mask = 0;
    for(unsigned int x = 0; x < BOARD_DIMENSION; ++x)
    {
        unsigned int value = data[getIndexForRowColumn(row, x)];
        if(value != 0)
            mask |= static_cast<uint16_t>(1u << value);
    }
    for(unsigned int y = 0; y < BOARD_DIMENSION; ++y)
    {
        unsigned int value = data[getIndexForRowColumn(y, column)];
        if(value != 0)
            mask |= static_cast<uint16_t>(1u << value);
    }

    unsigned int boxRow = (row / 3) * 3;
    unsigned int boxCol = (column / 3) * 3;
    for(unsigned int y = 0; y < 3; ++y)
        for(unsigned int x = 0; x < 3; ++x)
        {
            unsigned int value = data[getIndexForRowColumn(boxRow + y, boxCol + x)];
            if(value != 0)
                mask |= static_cast<uint16_t>(1u << value);
        }

    return mask;
}

std::vector<unsigned int> Board::getCandidatesForCell(const std::vector<unsigned int>& data, unsigned int row, unsigned int column) const
{
    uint16_t mask = getUsedMaskForCell(data, row, column);
    std::vector<unsigned int> candidates;
    candidates.reserve(BOARD_DIMENSION);
    for(unsigned int value = 1; value <= BOARD_DIMENSION; ++value)
    {
        if((mask & (1u << value)) == 0)
            candidates.push_back(value);
    }
    return candidates;
}

bool Board::solveBoard(std::vector<unsigned int>& data, bool randomize)
{
    unsigned int bestIndex = BOARD_DIMENSION * BOARD_DIMENSION;
    std::vector<unsigned int> bestCandidates;
    size_t bestCount = BOARD_DIMENSION + 1;

    for(unsigned int index = 0; index < BOARD_DIMENSION * BOARD_DIMENSION; ++index)
    {
        if(data[index] != 0)
            continue;

        unsigned int row = getRowForIndex(index);
        unsigned int column = getColumnForIndex(index);
        std::vector<unsigned int> candidates = getCandidatesForCell(data, row, column);

        if(candidates.empty())
            return false;

        if(candidates.size() < bestCount)
        {
            bestCount = candidates.size();
            bestIndex = index;
            bestCandidates = std::move(candidates);

            if(bestCount == 1)
                break;
        }
    }

    if(bestIndex == BOARD_DIMENSION * BOARD_DIMENSION)
        return true;

    if(randomize)
        std::shuffle(bestCandidates.begin(), bestCandidates.end(), _rng);

    for(unsigned int value : bestCandidates)
    {
        data[bestIndex] = value;
        if(solveBoard(data, randomize))
            return true;
        data[bestIndex] = 0;
    }

    return false;
}

unsigned int Board::countSolutionsImpl(std::vector<unsigned int>& data, unsigned int limit) const
{
    unsigned int bestIndex = BOARD_DIMENSION * BOARD_DIMENSION;
    std::vector<unsigned int> bestCandidates;
    size_t bestCount = BOARD_DIMENSION + 1;

    for(unsigned int index = 0; index < BOARD_DIMENSION * BOARD_DIMENSION; ++index)
    {
        if(data[index] != 0)
            continue;

        unsigned int row = getRowForIndex(index);
        unsigned int column = getColumnForIndex(index);
        std::vector<unsigned int> candidates = getCandidatesForCell(data, row, column);

        if(candidates.empty())
            return 0;

        if(candidates.size() < bestCount)
        {
            bestCount = candidates.size();
            bestIndex = index;
            bestCandidates = std::move(candidates);
            if(bestCount == 1)
                break;
        }
    }

    if(bestIndex == BOARD_DIMENSION * BOARD_DIMENSION)
        return 1;

    unsigned int total = 0;
    for(unsigned int value : bestCandidates)
    {
        data[bestIndex] = value;
        total += countSolutionsImpl(data, limit - total);
        data[bestIndex] = 0;

        if(total >= limit)
            return total;
    }

    return total;
}

const std::vector<unsigned int> Board::getRow(unsigned int index) const
{
    std::vector<unsigned int> row;
    
    for(unsigned int x = 0; x < BOARD_DIMENSION; ++x)
        row.push_back(_boardData->at(index * BOARD_DIMENSION + x));
    
    return row;
}
const std::vector<unsigned int> Board::getColumn(unsigned int index) const
{
    std::vector<unsigned int> column;
    
    for(unsigned int y = 0; y < BOARD_DIMENSION; ++y)
        column.push_back(_boardData->at(y * BOARD_DIMENSION + index));
    
    return column;
}

const std::vector<unsigned int> Board::getQuadrant(unsigned int index) const
{
    std::vector<unsigned int> quadrant;
    unsigned int boxRow = (index / 3) * 3;
    unsigned int boxCol = (index % 3) * 3;

    for(unsigned int y = 0; y < 3; ++y)
        for(unsigned int x = 0; x < 3; ++x)
            quadrant.push_back(_boardData->at((boxRow + y) * BOARD_DIMENSION + (boxCol + x)));
    
    return quadrant;
}

unsigned int Board::getRowForIndex(unsigned int index) const
{
    return index / BOARD_DIMENSION;
}

unsigned int Board::getColumnForIndex(unsigned int index) const
{
    return index % BOARD_DIMENSION;
}

unsigned int Board::getQuadrantForIndex(unsigned int index) const
{
    unsigned int row = getRowForIndex(index);
    unsigned int column = getColumnForIndex(index);
    
    unsigned int dimentionSqrt = std::sqrt(BOARD_DIMENSION);
    
    return column / dimentionSqrt + (row / dimentionSqrt) * dimentionSqrt;
}

bool Board::isVectorUnique(const std::vector<unsigned int> & data) const
{
    std::vector<unsigned int> sortedData = data;
    sort(sortedData.begin(), sortedData.end());
    return !(std::adjacent_find(sortedData.begin(), sortedData.end()) != sortedData.end());
}

std::ostream& sudoku::operator<<(std::ostream& os, const Board& board)
{
    os << "-------------------" << std::endl;
    for(unsigned int y = 0; y < Board::BOARD_DIMENSION; ++y)
    {
        os << '|';
        for(unsigned int x = 0; x < Board::BOARD_DIMENSION; ++x)
        {
            os << board._boardData->at(y * Board::BOARD_DIMENSION + x);
            if(x%3 == 2)
                os << '|';
            else
                os << ' ';
        }
        os << std::endl;
        if(y%3 == 2)
            os << "-------------------" << std::endl;
    }
    return os;
}
