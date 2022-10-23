//
//  BoardTest.h
//  Sudoku
//
//  Created by Marcin on 23/10/2022.
//

#ifndef BoardTest_h
#define BoardTest_h

#include <memory>
#include "gtest/gtest.h"

namespace sudoku {
class Board;

class BoardTest : public testing::Test
{
protected:
    void SetUp();
    void TearDown();
};

}

#endif /* BoardTest_h */
