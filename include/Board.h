//
//  Board.h
//  Sudoku
//
//  Created by Marcin on 23/10/2022.
//

#ifndef Board_h
#define Board_h

#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include <vector>

namespace sudoku {

class Board
{
private:
    std::unique_ptr<std::vector<unsigned int>> _boardData;
    std::mt19937 _rng;

    unsigned int getIndexForRowColumn(unsigned int row, unsigned int column) const;
    uint16_t getUsedMaskForCell(const std::vector<unsigned int>& data, unsigned int row, unsigned int column) const;
    std::vector<unsigned int> getCandidatesForCell(const std::vector<unsigned int>& data, unsigned int row, unsigned int column) const;
    bool solveBoard(std::vector<unsigned int>& data, bool randomize);
    unsigned int countSolutionsImpl(std::vector<unsigned int>& data, unsigned int limit) const;
public:
    static const unsigned int BOARD_DIMENSION = 9;
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
    inline const std::vector<unsigned int>& getBoardData() const {return *_boardData;}
};

}

#endif /* Board_h */
