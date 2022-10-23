//
//  main.cpp
//  Sudoku
//
//  Created by Marcin on 23/10/2022.
//

#include <iostream>
#include <memory>
#include "Board.h"

using namespace sudoku;

int main(int argc, const char * argv[]) {
    // insert code here...    
    std::unique_ptr<Board> board = std::make_unique<Board>();
    
    std::cout << *board;
    
    return 0;
}
