# Sudoku Generator

[![CI](https://github.com/elduty/sudoku/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/elduty/sudoku/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE.md)
[![codecov](https://codecov.io/gh/elduty/sudoku/branch/main/graph/badge.svg)](https://codecov.io/gh/elduty/sudoku)

A C++20 Sudoku board generator that can produce full solutions or playable puzzles with unique solutions. Build with CMake and run on macOS, Linux, or Windows.

## Features
- Randomized backtracking solver (guaranteed solution).
- Unique-solution puzzle generation with configurable clue count.
- Simple benchmarking harness.

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

## Project Layout
- `src/` and `include/`: core library and CLI.
- `tests/src/`: GoogleTest suite.
- `bench/`: benchmarking harness.
- `external/GoogleTest`: vendored GoogleTest.
