cmake_minimum_required(VERSION 3.16)
project(Sudoku VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SUDOKU_BUILD_TESTS "Build tests" ON)

# Core library
add_library(sudoku_lib
    src/Board.cpp
)
add_library(sudoku::lib ALIAS sudoku_lib)

target_include_directories(sudoku_lib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(sudoku_lib PUBLIC cxx_std_20)

target_compile_options(sudoku_lib PRIVATE
    $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Main executable
add_executable(sudoku
    src/main.cpp
)

target_link_libraries(sudoku PRIVATE sudoku::lib)

add_executable(sudokuBench
    bench/sudoku_bench.cpp
)
target_link_libraries(sudokuBench PRIVATE sudoku::lib)

# Tests
if(SUDOKU_BUILD_TESTS)
    include(CTest)
    enable_testing()

    # Silence deprecation warnings from vendored GoogleTest CMake files.
    set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

    add_subdirectory(external/GoogleTest)

    add_executable(sudokuTests
        tests/src/BoardTest.cpp
    )
    target_include_directories(sudokuTests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )
    target_link_libraries(sudokuTests PRIVATE
        sudoku::lib
        gtest
        gtest_main
    )

    add_test(NAME sudokuTests COMMAND sudokuTests)
endif()
