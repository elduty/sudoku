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

TEST(BoardTest, checkLines)
{
    Board board;
    const std::vector<unsigned int>& data = board.getBoardData();
    
    for(unsigned int y = 0; y < Board::BOARD_HIGHT; ++y)
    {
        std::vector<unsigned int> line;
        for(unsigned int x = 0; x < Board::BOARD_WIDTH; ++x)
        {
            line.push_back(data[y * Board::BOARD_HIGHT + x]);
        }
        EXPECT_EQ(isVectorUnique(line), true);
    }
}

TEST(BoardTest, checkColumns)
{
    Board board;
    const std::vector<unsigned int>& data = board.getBoardData();
    
    for(unsigned int x = 0; x < Board::BOARD_WIDTH; ++x)
    {
        std::vector<unsigned int> column;
        for(unsigned int y = 0; y < Board::BOARD_HIGHT; ++y)
        {
            column.push_back(data[y * Board::BOARD_HIGHT + x]);
        }
        EXPECT_EQ(isVectorUnique(column), true);
    }
}

TEST(BoardTest, checkQuadrants)
{
    Board board;
    const std::vector<unsigned int>& data = board.getBoardData();
    
    unsigned int iterator = 0;
    
    for(unsigned int qy = 0; qy < 3; ++qy)
    {
        for(unsigned int qx = 0; qx < 3; ++qx)
        {
            std::vector<unsigned int> quadrant;
            for(unsigned int y = 0; y < 3; ++y)
                for(unsigned int x = 0; x < 3; ++x)
                {
                    iterator = (y + qy * 3) * Board::BOARD_HIGHT + x + qx * 3;
                    quadrant.push_back(data[iterator]);
                }
            EXPECT_EQ(isVectorUnique(quadrant), true);
        }
    }
}

TEST(BoardTest, checkValidity)
{
    Board board;
    const std::vector<unsigned int>& data = board.getBoardData();
    
    unsigned int quadrantIterator = 0;
    unsigned int lineIterator = 0;
    unsigned int columnIterator = 0;
    
    for(unsigned int qy = 0; qy < 3; ++qy)
    {
        for(unsigned int qx = 0; qx < 3; ++qx)
        {
            std::vector<unsigned int> quadrant;
            std::vector<unsigned int> line;
            std::vector<unsigned int> column;
            for(unsigned int y = 0; y < 3; ++y)
                for(unsigned int x = 0; x < 3; ++x)
                {
                    quadrantIterator = (y + qy * 3) * Board::BOARD_HIGHT + x + qx * 3;
                    quadrant.push_back(data[quadrantIterator]);
                    
                    lineIterator = (qx + qy * 3) * Board::BOARD_HIGHT + x + y * 3;
                    line.push_back(data[lineIterator]);
                    
                    columnIterator = (x + y * 3) * Board::BOARD_HIGHT + qx + qy * 3;
                    column.push_back(data[columnIterator]);
                }
            EXPECT_EQ(isVectorUnique(quadrant), true);
            EXPECT_EQ(isVectorUnique(line), true);
            EXPECT_EQ(isVectorUnique(column), true);
        }
    }
}
