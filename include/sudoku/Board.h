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
    Board();
    ~Board();
    friend std::ostream& operator<<(std::ostream& os, const Board& board);
};

}

#endif /* Board_h */
