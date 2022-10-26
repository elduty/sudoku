//
//  Board.cpp
//  Sudoku
//
//  Created by Marcin on 23/10/2022.
//

#include <stdio.h>
#include "Board.h"
#include <cmath>

using namespace sudoku;

Board::Board():_boardData(std::make_unique<std::vector<unsigned int>>())
{
    _boardData->reserve(BOARD_DIMENSION * BOARD_DIMENSION);
    
    for(unsigned int y = 0; y < BOARD_DIMENSION; ++y)
        for(unsigned int x = 0; x < BOARD_DIMENSION; ++x)
        {
            _boardData->push_back(x + 1);
        }
}

Board::~Board()
{
    
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
    unsigned int dimentionSqrt = std::sqrt(BOARD_DIMENSION);
    unsigned int firstElementIndex = (index % BOARD_DIMENSION) * dimentionSqrt + (index / BOARD_DIMENSION) * dimentionSqrt;
    
    std::vector<unsigned int> quadrant;
    
    for(unsigned int y = 0; y < dimentionSqrt; ++y)
        for(unsigned int x = 0; x < dimentionSqrt; ++x)
            quadrant.push_back(_boardData->at(y * BOARD_DIMENSION + x + firstElementIndex));
    
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
