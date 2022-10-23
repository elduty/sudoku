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

TEST(BoardTest, checkLines)
{
    Board board;
    const std::vector<unsigned int>& data = board.getBoardData();
    EXPECT_EQ(isVectorUnique(data), true);
}

TEST(BoardTest, checkColumns)
{
    Board board;
    const std::vector<unsigned int>& data = board.getBoardData();
    EXPECT_EQ(isVectorUnique(data), true);
}

TEST(BoardTest, checkQuadrants)
{
    Board board;
    const std::vector<unsigned int>& data = board.getBoardData();
    EXPECT_EQ(isVectorUnique(data), true);
}
