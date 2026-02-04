# Sudoku Generator

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

## Benchmark

```bash
./build/sudokuBench --iterations 100
```

## Project Layout
- `src/` and `include/`: core library and CLI.
- `tests/src/`: GoogleTest suite.
- `bench/`: benchmarking harness.
- `external/GoogleTest`: vendored GoogleTest.
