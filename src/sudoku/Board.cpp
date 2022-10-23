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
    _boardData->reserve(BOARD_WIDTH * BOARD_HIGHT);
    for(unsigned int y = 0; y < BOARD_HIGHT; ++y)
        for(unsigned int x = 0; x < BOARD_WIDTH; ++x)
            _boardData->push_back(x + 1);
}

Board::~Board()
{
    
}

std::ostream& sudoku::operator<<(std::ostream& os, const Board& board)
{
    os << "-------------------" << std::endl;
    for(unsigned int y = 0; y < Board::BOARD_HIGHT; ++y)
    {
        os << '|';
        for(unsigned int x = 0; x < Board::BOARD_WIDTH; ++x)
        {
            os << board._boardData->at(y * Board::BOARD_HIGHT + x);
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
