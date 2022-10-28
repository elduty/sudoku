//
//  Board.cpp
//  Sudoku
//
//  Created by Marcin on 23/10/2022.
//

#include <stdio.h>
#include "Board.h"
#include <algorithm>
#include <cmath>

using namespace sudoku;

Board::Board():_boardData(std::make_unique<std::vector<unsigned int>>(BOARD_DIMENSION * BOARD_DIMENSION))
{
}

Board::~Board()
{
}

bool Board::generateBoard()
{
    std::fill(_boardData->begin(), _boardData->end(), 0);
    //srand (time(NULL));
    for(unsigned int y = 0; y < BOARD_DIMENSION; ++y)
        for(unsigned int x = 0; x < BOARD_DIMENSION; ++x)
        {
            unsigned int result = getLegalValueForIndex(y * BOARD_DIMENSION + x);
            
            if(result == 0)
                return false;
            
            _boardData->at(y * BOARD_DIMENSION + x) = result;
        }
    
    return true;
}

unsigned int Board::getLegalValueForIndex(unsigned int index) const
{
    std::vector<unsigned int> row = getRow(getRowForIndex(index));
    std::vector<unsigned int> column = getColumn(getColumnForIndex(index));
    std::vector<unsigned int> quadrant = getQuadrant(getQuadrantForIndex(index));
    
    std::sort(row.begin(), row.end());
    std::sort(column.begin(), column.end());
    std::sort(quadrant.begin(), quadrant.end());
    
    std::vector<unsigned int> unionRowColumn;
    std::set_union(row.begin(), row.end(), column.begin(), column.end(), std::back_inserter(unionRowColumn));
      
    std::vector<unsigned int> unionRowColumnQuadrant;
    std::set_union(unionRowColumn.begin(), unionRowColumn.end(), quadrant.begin(), quadrant.end(), std::back_inserter(unionRowColumnQuadrant));
    
    std::vector<unsigned int> possibleValues {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<unsigned int> legalResults;
    
    std::set_difference(possibleValues.begin(), possibleValues.end(), unionRowColumnQuadrant.begin(), unionRowColumnQuadrant.end(), std::back_inserter(legalResults));
    
    if(legalResults.empty())
        return 0;
    
    return legalResults.at(std::rand() % legalResults.size());
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
