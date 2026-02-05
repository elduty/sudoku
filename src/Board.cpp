//
//  Board.cpp
//  Sudoku
//
//  Created by Marcin on 23/10/2022.
//

#include <algorithm>
#include <array>
#include <bit>
#include <numeric>
#include <random>
#include "Board.h"

using namespace sudoku;

namespace {

constexpr unsigned int kBoardCells = Board::BOARD_DIMENSION * Board::BOARD_DIMENSION;
constexpr unsigned int kBoxDimension = 3;
constexpr uint16_t kAllDigitsMask = static_cast<uint16_t>((1u << (Board::BOARD_DIMENSION + 1)) - 2u);

inline unsigned int rowForIndex(unsigned int index)
{
    return index / Board::BOARD_DIMENSION;
}

inline unsigned int columnForIndex(unsigned int index)
{
    return index % Board::BOARD_DIMENSION;
}

inline unsigned int boxForRowColumn(unsigned int row, unsigned int column)
{
    return (row / kBoxDimension) * kBoxDimension + (column / kBoxDimension);
}

} // namespace

Board::Board(std::optional<uint32_t> seed)
    : _boardData(kBoardCells, 0),
      _workBuffer(kBoardCells, 0),
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

bool Board::initializeMasks(const std::vector<unsigned int>& data,
                            std::array<uint16_t, BOARD_DIMENSION>& rowMasks,
                            std::array<uint16_t, BOARD_DIMENSION>& columnMasks,
                            std::array<uint16_t, BOARD_DIMENSION>& boxMasks) const
{
    rowMasks.fill(0);
    columnMasks.fill(0);
    boxMasks.fill(0);

    for(unsigned int index = 0; index < kBoardCells; ++index)
    {
        const unsigned int value = data[index];
        if(value == 0)
            continue;
        if(value > BOARD_DIMENSION)
            return false;

        const unsigned int row = rowForIndex(index);
        const unsigned int column = columnForIndex(index);
        const unsigned int box = boxForRowColumn(row, column);
        const uint16_t bit = static_cast<uint16_t>(1u << value);

        if((rowMasks[row] & bit) != 0 || (columnMasks[column] & bit) != 0 || (boxMasks[box] & bit) != 0)
            return false;

        rowMasks[row] |= bit;
        columnMasks[column] |= bit;
        boxMasks[box] |= bit;
    }

    return true;
}

bool Board::findBestCell(const std::vector<unsigned int>& data,
                         const std::array<uint16_t, BOARD_DIMENSION>& rowMasks,
                         const std::array<uint16_t, BOARD_DIMENSION>& columnMasks,
                         const std::array<uint16_t, BOARD_DIMENSION>& boxMasks,
                         unsigned int& bestIndex,
                         uint16_t& bestCandidateMask) const
{
    unsigned int bestCount = BOARD_DIMENSION + 1;
    bestIndex = kBoardCells;
    bestCandidateMask = 0;

    for(unsigned int index = 0; index < kBoardCells; ++index)
    {
        if(data[index] != 0)
            continue;

        const unsigned int row = rowForIndex(index);
        const unsigned int column = columnForIndex(index);
        const unsigned int box = boxForRowColumn(row, column);
        const uint16_t used = static_cast<uint16_t>(rowMasks[row] | columnMasks[column] | boxMasks[box]);
        const uint16_t candidates = static_cast<uint16_t>(kAllDigitsMask & ~used);

        if(candidates == 0)
            return false;

        const unsigned int count = std::popcount(candidates);
        if(count < bestCount)
        {
            bestCount = count;
            bestIndex = index;
            bestCandidateMask = candidates;
            if(bestCount == 1)
                break;
        }
    }

    return true;
}

bool Board::solveBoard(std::vector<unsigned int>& data,
                       std::array<uint16_t, BOARD_DIMENSION>& rowMasks,
                       std::array<uint16_t, BOARD_DIMENSION>& columnMasks,
                       std::array<uint16_t, BOARD_DIMENSION>& boxMasks,
                       bool randomize)
{
    unsigned int bestIndex = kBoardCells;
    uint16_t candidateMask = 0;
    if(!findBestCell(data, rowMasks, columnMasks, boxMasks, bestIndex, candidateMask))
        return false;

    if(bestIndex == kBoardCells)
        return true;

    std::array<unsigned int, BOARD_DIMENSION> candidates{};
    unsigned int candidateCount = 0;
    for(unsigned int value = 1; value <= BOARD_DIMENSION; ++value)
    {
        if((candidateMask & (1u << value)) != 0)
            candidates[candidateCount++] = value;
    }

    if(randomize)
        std::shuffle(candidates.begin(), candidates.begin() + candidateCount, _rng);

    const unsigned int row = rowForIndex(bestIndex);
    const unsigned int column = columnForIndex(bestIndex);
    const unsigned int box = boxForRowColumn(row, column);

    for(unsigned int i = 0; i < candidateCount; ++i)
    {
        const unsigned int value = candidates[i];
        const uint16_t bit = static_cast<uint16_t>(1u << value);

        data[bestIndex] = value;
        rowMasks[row] |= bit;
        columnMasks[column] |= bit;
        boxMasks[box] |= bit;

        if(solveBoard(data, rowMasks, columnMasks, boxMasks, randomize))
            return true;

        boxMasks[box] &= static_cast<uint16_t>(~bit);
        columnMasks[column] &= static_cast<uint16_t>(~bit);
        rowMasks[row] &= static_cast<uint16_t>(~bit);
        data[bestIndex] = 0;
    }

    return false;
}

bool Board::solveBoard(std::vector<unsigned int>& data, bool randomize)
{
    std::array<uint16_t, BOARD_DIMENSION> rowMasks{};
    std::array<uint16_t, BOARD_DIMENSION> columnMasks{};
    std::array<uint16_t, BOARD_DIMENSION> boxMasks{};
    if(!initializeMasks(data, rowMasks, columnMasks, boxMasks))
        return false;

    return solveBoard(data, rowMasks, columnMasks, boxMasks, randomize);
}

bool Board::generateSolution()
{
    std::fill(_boardData.begin(), _boardData.end(), 0);
    return solveBoard(_boardData, true);
}

unsigned int Board::countSolutionsImpl(std::vector<unsigned int>& data,
                                       std::array<uint16_t, BOARD_DIMENSION>& rowMasks,
                                       std::array<uint16_t, BOARD_DIMENSION>& columnMasks,
                                       std::array<uint16_t, BOARD_DIMENSION>& boxMasks,
                                       unsigned int limit) const
{
    unsigned int bestIndex = kBoardCells;
    uint16_t candidateMask = 0;
    if(!findBestCell(data, rowMasks, columnMasks, boxMasks, bestIndex, candidateMask))
        return 0;

    if(bestIndex == kBoardCells)
        return 1;

    const unsigned int row = rowForIndex(bestIndex);
    const unsigned int column = columnForIndex(bestIndex);
    const unsigned int box = boxForRowColumn(row, column);

    unsigned int total = 0;
    for(unsigned int value = 1; value <= BOARD_DIMENSION; ++value)
    {
        const uint16_t bit = static_cast<uint16_t>(1u << value);
        if((candidateMask & bit) == 0)
            continue;

        data[bestIndex] = value;
        rowMasks[row] |= bit;
        columnMasks[column] |= bit;
        boxMasks[box] |= bit;

        total += countSolutionsImpl(data, rowMasks, columnMasks, boxMasks, limit - total);

        boxMasks[box] &= static_cast<uint16_t>(~bit);
        columnMasks[column] &= static_cast<uint16_t>(~bit);
        rowMasks[row] &= static_cast<uint16_t>(~bit);
        data[bestIndex] = 0;

        if(total >= limit)
            return total;
    }

    return total;
}

unsigned int Board::countSolutionsOnData(std::vector<unsigned int>& data, unsigned int limit) const
{
    if(limit == 0)
        return 0;

    std::array<uint16_t, BOARD_DIMENSION> rowMasks{};
    std::array<uint16_t, BOARD_DIMENSION> columnMasks{};
    std::array<uint16_t, BOARD_DIMENSION> boxMasks{};
    if(!initializeMasks(data, rowMasks, columnMasks, boxMasks))
        return 0;

    return countSolutionsImpl(data, rowMasks, columnMasks, boxMasks, limit);
}

bool Board::generatePuzzle(unsigned int clues)
{
    if(clues > BOARD_DIMENSION * BOARD_DIMENSION)
        return false;

    if(!generateSolution())
        return false;

    std::array<unsigned int, kBoardCells> indices{};
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), _rng);

    unsigned int remaining = kBoardCells;
    for(unsigned int index : indices)
    {
        if(remaining <= clues)
            break;

        const unsigned int previous = _boardData[index];
        _boardData[index] = 0;

        if(countSolutionsOnData(_boardData, 2) == 1)
        {
            --remaining;
        }
        else
        {
            _boardData[index] = previous;
        }
    }

    return remaining == clues;
}

unsigned int Board::countSolutions(unsigned int limit) const
{
    _workBuffer = _boardData;
    return countSolutionsOnData(_workBuffer, limit);
}

unsigned int Board::getIndexForRowColumn(unsigned int row, unsigned int column) const
{
    return row * BOARD_DIMENSION + column;
}

std::vector<unsigned int> Board::getRow(unsigned int index) const
{
    if(index >= BOARD_DIMENSION)
        throw std::out_of_range("Row index out of range");

    std::vector<unsigned int> row(BOARD_DIMENSION);
    const unsigned int start = index * BOARD_DIMENSION;
    std::copy_n(_boardData.begin() + start, BOARD_DIMENSION, row.begin());
    return row;
}

std::vector<unsigned int> Board::getColumn(unsigned int index) const
{
    if(index >= BOARD_DIMENSION)
        throw std::out_of_range("Column index out of range");

    std::vector<unsigned int> column(BOARD_DIMENSION);
    for(unsigned int y = 0; y < BOARD_DIMENSION; ++y)
        column[y] = _boardData[y * BOARD_DIMENSION + index];

    return column;
}

std::vector<unsigned int> Board::getQuadrant(unsigned int index) const
{
    if(index >= BOARD_DIMENSION)
        throw std::out_of_range("Quadrant index out of range");

    std::vector<unsigned int> quadrant;
    quadrant.reserve(BOARD_DIMENSION);

    const unsigned int boxRow = (index / kBoxDimension) * kBoxDimension;
    const unsigned int boxCol = (index % kBoxDimension) * kBoxDimension;

    for(unsigned int y = 0; y < kBoxDimension; ++y)
        for(unsigned int x = 0; x < kBoxDimension; ++x)
            quadrant.push_back(_boardData[(boxRow + y) * BOARD_DIMENSION + (boxCol + x)]);

    return quadrant;
}

unsigned int Board::getRowForIndex(unsigned int index) const
{
    if(index >= kBoardCells)
        throw std::out_of_range("Cell index out of range");

    return rowForIndex(index);
}

unsigned int Board::getColumnForIndex(unsigned int index) const
{
    if(index >= kBoardCells)
        throw std::out_of_range("Cell index out of range");

    return columnForIndex(index);
}

unsigned int Board::getQuadrantForIndex(unsigned int index) const
{
    if(index >= kBoardCells)
        throw std::out_of_range("Cell index out of range");

    const unsigned int row = rowForIndex(index);
    const unsigned int column = columnForIndex(index);

    return column / kBoxDimension + (row / kBoxDimension) * kBoxDimension;
}

bool Board::isVectorUnique(const std::vector<unsigned int> & data) const
{
    uint16_t seen = 0;
    for(unsigned int value : data)
    {
        if(value == 0)
            continue;

        if(value > BOARD_DIMENSION)
            return false;

        const uint16_t bit = static_cast<uint16_t>(1u << value);
        if((seen & bit) != 0)
            return false;
        seen |= bit;
    }

    return true;
}

std::ostream& sudoku::operator<<(std::ostream& os, const Board& board)
{
    os << "-------------------\n";
    for(unsigned int y = 0; y < Board::BOARD_DIMENSION; ++y)
    {
        os << '|';
        for(unsigned int x = 0; x < Board::BOARD_DIMENSION; ++x)
        {
            os << board._boardData[y * Board::BOARD_DIMENSION + x];
            if(x % kBoxDimension == kBoxDimension - 1)
                os << '|';
            else
                os << ' ';
        }
        os << '\n';
        if(y % kBoxDimension == kBoxDimension - 1)
            os << "-------------------\n";
    }
    return os;
}
