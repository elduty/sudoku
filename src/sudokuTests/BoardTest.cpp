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

TEST(BoardTest, checkGetRowForIndex)
{
    Board board;
    
    EXPECT_EQ(board.getRowForIndex(0), 0);
    EXPECT_EQ(board.getRowForIndex(10), 1);
    EXPECT_EQ(board.getRowForIndex(20), 2);
    EXPECT_EQ(board.getRowForIndex(30), 3);
    EXPECT_EQ(board.getRowForIndex(40), 4);
    EXPECT_EQ(board.getRowForIndex(50), 5);
    EXPECT_EQ(board.getRowForIndex(60), 6);
    EXPECT_EQ(board.getRowForIndex(70), 7);
    EXPECT_EQ(board.getRowForIndex(80), 8);
}

TEST(BoardTest, checkGetColumnForIndex)
{
    Board board;
    
    EXPECT_EQ(board.getColumnForIndex(0), 0);
    EXPECT_EQ(board.getRowForIndex(10), 1);
    EXPECT_EQ(board.getRowForIndex(20), 2);
    EXPECT_EQ(board.getRowForIndex(30), 3);
    EXPECT_EQ(board.getRowForIndex(40), 4);
    EXPECT_EQ(board.getRowForIndex(50), 5);
    EXPECT_EQ(board.getRowForIndex(60), 6);
    EXPECT_EQ(board.getRowForIndex(70), 7);
    EXPECT_EQ(board.getRowForIndex(80), 8);
}

TEST(BoardTest, checkQuadrantForIndex)
{
    Board board;
    
    EXPECT_EQ(board.getQuadrantForIndex(0), 0);
    EXPECT_EQ(board.getQuadrantForIndex(13), 1);
    EXPECT_EQ(board.getQuadrantForIndex(26), 2);
    EXPECT_EQ(board.getQuadrantForIndex(45), 3);
    EXPECT_EQ(board.getQuadrantForIndex(32), 4);
    EXPECT_EQ(board.getQuadrantForIndex(42), 5);
    EXPECT_EQ(board.getQuadrantForIndex(65), 6);
    EXPECT_EQ(board.getQuadrantForIndex(58), 7);
    EXPECT_EQ(board.getQuadrantForIndex(79), 8);
}

TEST(BoardTest, checkValidity)
{
    Board board;
    
    EXPECT_EQ(board.isVectorUnique(std::vector<unsigned int>{1, 2, 3, 4, 5, 6, 7, 8}), true);
    EXPECT_EQ(board.isVectorUnique(std::vector<unsigned int>{1, 2, 3, 4, 4, 6, 7, 8}), false);
}
