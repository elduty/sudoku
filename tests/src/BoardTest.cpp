#include <algorithm>
#include <array>
#include <vector>

#include "gtest/gtest.h"
#include "Board.h"

using namespace sudoku;

namespace {

std::array<unsigned int, 9> sortedRow(const Board& board, unsigned int index)
{
    std::array<unsigned int, 9> row{};
    std::vector<unsigned int> values = board.getRow(index);
    std::sort(values.begin(), values.end());
    std::copy(values.begin(), values.end(), row.begin());
    return row;
}

std::array<unsigned int, 9> sortedColumn(const Board& board, unsigned int index)
{
    std::array<unsigned int, 9> column{};
    std::vector<unsigned int> values = board.getColumn(index);
    std::sort(values.begin(), values.end());
    std::copy(values.begin(), values.end(), column.begin());
    return column;
}

std::array<unsigned int, 9> sortedQuadrant(const Board& board, unsigned int index)
{
    std::array<unsigned int, 9> quadrant{};
    std::vector<unsigned int> values = board.getQuadrant(index);
    std::sort(values.begin(), values.end());
    std::copy(values.begin(), values.end(), quadrant.begin());
    return quadrant;
}

std::array<unsigned int, 9> expectedDigits()
{
    return {1, 2, 3, 4, 5, 6, 7, 8, 9};
}

} // namespace

TEST(BoardTest, generateSolutionProducesValidBoard)
{
    Board board(1234);
    EXPECT_TRUE(board.generateSolution());

    const auto expected = expectedDigits();
    for(unsigned int index = 0; index < Board::BOARD_DIMENSION; ++index)
    {
        EXPECT_EQ(sortedRow(board, index), expected);
        EXPECT_EQ(sortedColumn(board, index), expected);
        EXPECT_EQ(sortedQuadrant(board, index), expected);
    }
}

TEST(BoardTest, generatePuzzleProducesUniqueBoardWithClueCount)
{
    Board board(5678);
    const unsigned int clueTarget = 30;
    EXPECT_TRUE(board.generatePuzzle(clueTarget));

    const std::vector<unsigned int>& data = board.getBoardData();
    unsigned int clues = 0;
    for(auto cell : data)
    {
        if(cell != 0)
            ++clues;
    }

    EXPECT_EQ(clues, clueTarget);
    EXPECT_EQ(board.countSolutions(2), 1u);
}
