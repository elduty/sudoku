//
//  Board.h
//  Sudoku
//
//  Created by Marcin on 23/10/2022.
//

#ifndef Board_h
#define Board_h

#include <memory>
#include <vector>
#include <iostream>

namespace sudoku {

class Board
{
private:
    std::unique_ptr<std::vector<unsigned int>> _boardData;
public:
    static const unsigned int BOARD_DIMENSION = 9;
    Board();
    ~Board();
    
    const std::vector<unsigned int> getRow(unsigned int) const;
    const std::vector<unsigned int> getColumn(unsigned int) const;
    const std::vector<unsigned int> getQuadrant(unsigned int) const;
    
    unsigned int getRowForIndex(unsigned int) const;
    unsigned int getColumnForIndex(unsigned int) const;
    unsigned int getQuadrantForIndex(unsigned int) const;
    
    bool isVectorUnique(const std::vector<unsigned int>&) const;
    
    friend std::ostream& operator<<(std::ostream&, const Board&);
    inline const std::vector<unsigned int>& getBoardData() const {return *_boardData;}
};

}

#endif /* Board_h */
