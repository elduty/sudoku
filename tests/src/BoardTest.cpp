#include <algorithm>
#include <array>
#include <sstream>
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

TEST(BoardTest, constructorWithoutSeedCreatesBoard)
{
    Board board;
    EXPECT_EQ(board.getBoardData().size(), Board::BOARD_DIMENSION * Board::BOARD_DIMENSION);
}

TEST(BoardTest, setSeedChangesBoardGeneration)
{
    Board board1(1111);
    board1.generateSolution();
    auto data1 = board1.getBoardData();

    Board board2(2222);
    board2.generateSolution();
    board2.setSeed(1111);
    board2.generateSolution();
    auto data2 = board2.getBoardData();

    EXPECT_EQ(data1, data2);
}

TEST(BoardTest, getRowForIndexReturnsCorrectRow)
{
    Board board;
    EXPECT_EQ(board.getRowForIndex(0), 0u);
    EXPECT_EQ(board.getRowForIndex(8), 0u);
    EXPECT_EQ(board.getRowForIndex(9), 1u);
    EXPECT_EQ(board.getRowForIndex(80), 8u);
}

TEST(BoardTest, getColumnForIndexReturnsCorrectColumn)
{
    Board board;
    EXPECT_EQ(board.getColumnForIndex(0), 0u);
    EXPECT_EQ(board.getColumnForIndex(8), 8u);
    EXPECT_EQ(board.getColumnForIndex(9), 0u);
    EXPECT_EQ(board.getColumnForIndex(80), 8u);
}

TEST(BoardTest, getQuadrantForIndexReturnsCorrectQuadrant)
{
    Board board;
    EXPECT_EQ(board.getQuadrantForIndex(0), 0u);
    EXPECT_EQ(board.getQuadrantForIndex(3), 1u);
    EXPECT_EQ(board.getQuadrantForIndex(27), 3u);
    EXPECT_EQ(board.getQuadrantForIndex(40), 4u);
    EXPECT_EQ(board.getQuadrantForIndex(80), 8u);
}

TEST(BoardTest, isVectorUniqueReturnsTrueForUniqueValues)
{
    Board board;
    std::vector<unsigned int> unique = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_TRUE(board.isVectorUnique(unique));
}

TEST(BoardTest, isVectorUniqueReturnsFalseForDuplicates)
{
    Board board;
    std::vector<unsigned int> duplicates = {1, 2, 3, 4, 5, 5, 7, 8, 9};
    EXPECT_FALSE(board.isVectorUnique(duplicates));
}

TEST(BoardTest, generatePuzzleFailsWithTooManyClues)
{
    Board board(9999);
    EXPECT_FALSE(board.generatePuzzle(82));
}

TEST(BoardTest, streamOutputProducesFormattedBoard)
{
    Board board(4242);
    board.generateSolution();

    std::ostringstream oss;
    oss << board;

    std::string output = oss.str();
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("-------------------"), std::string::npos);
    EXPECT_NE(output.find("|"), std::string::npos);
}

TEST(BoardTest, differentSeedsProduceDifferentSolutions)
{
    Board board1(1234);
    board1.generateSolution();

    Board board2(5678);
    board2.generateSolution();

    EXPECT_NE(board1.getBoardData(), board2.getBoardData());
}

TEST(BoardTest, countSolutionsReturnsZeroWhenLimitIsZero)
{
    Board board(1212);
    ASSERT_TRUE(board.generateSolution());
    EXPECT_EQ(board.countSolutions(0), 0u);
}

TEST(BoardTest, countSolutionsReturnsZeroForInvalidDigitOnBoard)
{
    Board board;
    board.getBoardData()[0] = 10;
    EXPECT_EQ(board.countSolutions(2), 0u);
}

TEST(BoardTest, countSolutionsReturnsZeroForConflictingDigits)
{
    Board board;
    board.getBoardData()[0] = 1;
    board.getBoardData()[1] = 1;
    EXPECT_EQ(board.countSolutions(2), 0u);
}

TEST(BoardTest, generatePuzzleWithFullCluesKeepsSolvedBoard)
{
    Board board(2020);
    ASSERT_TRUE(board.generatePuzzle(Board::BOARD_DIMENSION * Board::BOARD_DIMENSION));

    unsigned int clues = 0;
    for(unsigned int value : board.getBoardData())
    {
        if(value != 0)
            ++clues;
    }

    EXPECT_EQ(clues, Board::BOARD_DIMENSION * Board::BOARD_DIMENSION);
    EXPECT_EQ(board.countSolutions(2), 1u);
}

TEST(BoardTest, getRowColumnAndQuadrantReadExpectedCells)
{
    Board board;
    std::vector<unsigned int>& data = board.getBoardData();
    for(unsigned int i = 0; i < data.size(); ++i)
        data[i] = i + 1;

    EXPECT_EQ(board.getRow(2)[0], 19u);
    EXPECT_EQ(board.getRow(2)[8], 27u);
    EXPECT_EQ(board.getColumn(4)[0], 5u);
    EXPECT_EQ(board.getColumn(4)[8], 77u);
    EXPECT_EQ(board.getQuadrant(4)[0], 31u);
    EXPECT_EQ(board.getQuadrant(4)[8], 51u);
}

TEST(BoardTest, isVectorUniqueReturnsFalseWhenValueExceedsBoardDimension)
{
    Board board;
    std::vector<unsigned int> invalid = {1, 2, 3, 4, 5, 6, 7, 8, 10};
    EXPECT_FALSE(board.isVectorUnique(invalid));
}
