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
    _board = std::make_unique<Board>();
}

void BoardTest::TearDown()
{
}

TEST(BoardTest, checkLines)
{
    EXPECT_EQ(true, false);
}

TEST(BoardTest, checkColumns)
{
    EXPECT_EQ(true, false);
}

TEST(BoardTest, checkQuadrants)
{
    EXPECT_EQ(true, false);
}
