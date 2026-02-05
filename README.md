# Sudoku Generator

[![CI](https://github.com/elduty/sudoku/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/elduty/sudoku/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE.md)
[![codecov](https://codecov.io/gh/elduty/sudoku/branch/main/graph/badge.svg)](https://codecov.io/gh/elduty/sudoku)

A C++20 Sudoku board generator that can produce full solutions or playable puzzles with unique solutions. Build with CMake and run on macOS, Linux, or Windows.

## Features
- Backtracking solver with MRV (minimum remaining values) cell selection.
- Incremental row/column/box bitmask constraints for fast legality checks.
- Unique-solution puzzle generation with configurable clue count.
- Simple benchmarking harness.

## How It Works

### Solution generation
1. Start from an empty 9x9 board.
2. Keep three constraint masks (`row`, `column`, `box`) where bit `1<<digit` marks used digits.
3. Pick the next empty cell using MRV (the cell with the fewest legal candidates).
4. Build candidate digits from bitmasks, shuffle candidate order (for randomness), and recurse.
5. On failure, backtrack by clearing the cell and restoring masks.
6. Finish when no empty cells remain.

### Puzzle generation
1. Generate a full solved board.
2. Shuffle all 81 indices and try removing values one by one.
3. After each removal, count solutions with an early-stop limit of 2.
4. Keep the removal only if exactly one solution remains.
5. Stop when the requested clue count is reached.

## Build
From the repository root:

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Run
Generate a puzzle (default 30 clues):

```bash
./build/sudoku
```

Generate a full solution:

```bash
./build/sudoku --solution
```

Set clue count or a deterministic seed:

```bash
./build/sudoku --clues 35
./build/sudoku --seed 1234
./build/sudoku --clues 30 --seed 42
```

## Tests

```bash
ctest --test-dir build --output-on-failure
```

## Coverage

Code coverage is collected on CI and uploaded to [Codecov](https://codecov.io/gh/elduty/sudoku). To generate coverage locally:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage"
cmake --build build
ctest --test-dir build
lcov --capture --directory build --output-file coverage.info --ignore-errors mismatch
lcov --remove coverage.info '/usr/*' '*/external/*' '*/tests/*' --output-file coverage.info --ignore-errors mismatch
```

## Benchmark

```bash
./build/sudokuBench --iterations 100
```

The benchmark reports average/min/max solution generation time in milliseconds.

## Project Layout
- `src/` and `include/`: core library and CLI.
- `tests/src/`: GoogleTest suite.
- `bench/`: benchmarking harness.
- `external/GoogleTest`: vendored GoogleTest.
