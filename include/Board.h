//
//  Board.h
//  Sudoku
//
//  Created by Marcin on 23/10/2022.
//

#ifndef Board_h
#define Board_h

#include <array>
#include <cstdint>
#include <iostream>
#include <optional>
#include <random>
#include <vector>

namespace sudoku {

class Board
{
public:
    static const unsigned int BOARD_DIMENSION = 9;

private:
    std::vector<unsigned int> _boardData;
    std::mt19937 _rng;

    unsigned int getIndexForRowColumn(unsigned int row, unsigned int column) const;
    bool initializeMasks(const std::vector<unsigned int>& data,
                         std::array<uint16_t, BOARD_DIMENSION>& rowMasks,
                         std::array<uint16_t, BOARD_DIMENSION>& columnMasks,
                         std::array<uint16_t, BOARD_DIMENSION>& boxMasks) const;
    bool findBestCell(const std::vector<unsigned int>& data,
                      const std::array<uint16_t, BOARD_DIMENSION>& rowMasks,
                      const std::array<uint16_t, BOARD_DIMENSION>& columnMasks,
                      const std::array<uint16_t, BOARD_DIMENSION>& boxMasks,
                      unsigned int& bestIndex,
                      uint16_t& bestCandidateMask) const;
    bool solveBoard(std::vector<unsigned int>& data,
                    std::array<uint16_t, BOARD_DIMENSION>& rowMasks,
                    std::array<uint16_t, BOARD_DIMENSION>& columnMasks,
                    std::array<uint16_t, BOARD_DIMENSION>& boxMasks,
                    bool randomize);
    bool solveBoard(std::vector<unsigned int>& data, bool randomize);
    unsigned int countSolutionsOnData(std::vector<unsigned int>& data, unsigned int limit) const;
    unsigned int countSolutionsImpl(std::vector<unsigned int>& data,
                                    std::array<uint16_t, BOARD_DIMENSION>& rowMasks,
                                    std::array<uint16_t, BOARD_DIMENSION>& columnMasks,
                                    std::array<uint16_t, BOARD_DIMENSION>& boxMasks,
                                    unsigned int limit) const;

public:
    explicit Board(std::optional<uint32_t> seed = std::nullopt);
    ~Board();

    void setSeed(uint32_t seed);

    const std::vector<unsigned int> getRow(unsigned int) const;
    const std::vector<unsigned int> getColumn(unsigned int) const;
    const std::vector<unsigned int> getQuadrant(unsigned int) const;

    unsigned int getRowForIndex(unsigned int) const;
    unsigned int getColumnForIndex(unsigned int) const;
    unsigned int getQuadrantForIndex(unsigned int) const;

    bool isVectorUnique(const std::vector<unsigned int>&) const;
    bool generateSolution();
    bool generatePuzzle(unsigned int clues = 30);
    unsigned int countSolutions(unsigned int limit = 2) const;

    friend std::ostream& operator<<(std::ostream&, const Board&);
    inline const std::vector<unsigned int>& getBoardData() const {return _boardData;}
};

}

#endif /* Board_h */
