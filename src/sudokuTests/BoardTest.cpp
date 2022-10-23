//
//  BoardTest.cpp
//  SudokuTests
//
//  Created by Marcin on 23/10/2022.
//

#include <stdio.h>
#include <memory>
#include "BoardTest.h"
#include "Board.h"

using namespace sudoku;

void BoardTest::SetUp()
{
}

void BoardTest::TearDown()
{
}

TEST_F(BoardTest, checkBoard)
{
    std::unique_ptr<Board> board = std::make_unique<Board>();
    EXPECT_EQ(true, false);
}
