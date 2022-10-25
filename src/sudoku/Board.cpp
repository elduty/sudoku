//
//  Board.cpp
//  Sudoku
//
//  Created by Marcin on 23/10/2022.
//

#include <stdio.h>
#include "Board.h"

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

const std::vector<unsigned int> Board::getQuadrantFromIndex(unsigned int index) const
{
    std::vector<unsigned int> quadrant;
    
    for(unsigned int x = 0; x < 3; ++x)
        for(unsigned int y = 0; y < BOARD_DIMENSION; ++y)
            quadrant.push_back(_boardData->at(index + y * BOARD_DIMENSION + x));
    
    return quadrant;
}

const std::vector<unsigned int> Board::getQuadrant(unsigned int index) const
{
    switch (index) {
        case 0:
            return getQuadrantFromIndex(0);
            
        case 1:
            return getQuadrantFromIndex(3);
            
        case 2:
            return getQuadrantFromIndex(6);
            
        case 3:
            return getQuadrantFromIndex(27);
            
        case 4:
            return getQuadrantFromIndex(30);
            
        case 5:
            return getQuadrantFromIndex(33);
            
        case 6:
            return getQuadrantFromIndex(54);
            
        case 7:
            return getQuadrantFromIndex(57);
            
        case 8:
            return getQuadrantFromIndex(60);
            
        default:
            return std::vector<unsigned int> {0,0,0,0,0,0,0,0,0};
    }
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
    return 0;
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
