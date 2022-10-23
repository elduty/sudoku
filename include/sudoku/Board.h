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
    static const unsigned int BOARD_WIDTH = 9;
    static const unsigned int BOARD_HIGHT = 9;
    Board();
    ~Board();
    friend std::ostream& operator<<(std::ostream&, const Board&);
    inline const std::vector<unsigned int>& getBoardData() const {return *_boardData;}
};

}

#endif /* Board_h */
