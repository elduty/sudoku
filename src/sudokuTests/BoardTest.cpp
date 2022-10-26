//
//  BoardTest.cpp
//  SudokuTests
//
//  Created by Marcin on 23/10/2022.
//

#include <stdio.h>
#include "BoardTest.h"
#include "Board.h"

using namespace sudoku;

void BoardTest::SetUp()
{
}

void BoardTest::TearDown()
{
}

bool isVectorUnique(const std::vector<unsigned int> & data)
{
    std::vector<unsigned int> sortedData = data;
    sort(sortedData.begin(), sortedData.end());
    return !(std::adjacent_find(sortedData.begin(), sortedData.end()) != sortedData.end());
}

TEST(BoardTest, printBoard)
{
    Board board;
    std::cout << board;
}

TEST(BoardTest, checkRange)
{
    Board board;
    const std::vector<unsigned int>& data = board.getBoardData();
    for(auto cell : data)
    {
        EXPECT_GT(cell, 0);
        EXPECT_LE(cell, 9);
    }
}

TEST(BoardTest, checkRows)
{
    Board board;
    
    for(unsigned int index = 0; index < Board::BOARD_DIMENSION; ++index)
    {
        std::vector<unsigned int> row = board.getRow(index);
        
        EXPECT_EQ(isVectorUnique(row), true);
    }
}

TEST(BoardTest, checkColumns)
{
    Board board;
    
    for(unsigned int index = 0; index < Board::BOARD_DIMENSION; ++index)
    {
        std::vector<unsigned int> column = board.getColumn(index);
        
        EXPECT_EQ(isVectorUnique(column), true);
    }
}

TEST(BoardTest, checkQuadrants)
{
    Board board;
    
    for(unsigned int index = 0; index < Board::BOARD_DIMENSION; ++index)
    {
        std::vector<unsigned int> quadrant = board.getQuadrant(index);
        
        EXPECT_EQ(isVectorUnique(quadrant), true);
    }
}

TEST(BoardTest, checkValidity)
{
    Board board;
    
    for(unsigned int index = 0; index < Board::BOARD_DIMENSION; ++index)
    {
        std::vector<unsigned int> row = board.getRow(index);
        std::vector<unsigned int> column = board.getColumn(index);
        std::vector<unsigned int> quadrant = board.getQuadrant(index);
        
        EXPECT_EQ(isVectorUnique(row), true);
        EXPECT_EQ(isVectorUnique(column), true);
        EXPECT_EQ(isVectorUnique(quadrant), true);
    }
}
