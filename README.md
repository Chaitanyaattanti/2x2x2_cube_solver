Certainly! Here’s an **updated README** that matches the code you provided. The description now reflects your code’s actual features, move set, input/output format, and approach (bidirectional BFS, direct state encoding, etc.), and removes references to permutation/orientation encoding which your code does not use.

# 2x2x2 Rubik's Cube Solver in C++

This project implements a solver for the 2x2x2 Rubik’s Cube (Pocket Cube) using **Bidirectional Breadth-First Search (BFS)**. The cube's state is represented as a flat array of 24 stickers, and all standard face moves are supported.

## Features

- Solves any valid 2x2x2 cube configuration
- Finds the shortest sequence of moves using bidirectional BFS
- Supports all 18 standard face moves: F, F', F2, B, B', B2, R, R', R2, L, L', L2, U, U', U2, D, D', D2
- Validates input for correct color counts and format
- Efficient state encoding for fast lookup

## How It Works

- The cube is represented as a 24-character string (each sticker's color)
- Each move permutes and rotates stickers according to standard 2x2x2 cube notation
- Bidirectional BFS searches from the scrambled state and the solved state simultaneously for efficiency
- Once a solution is found, the move sequence is printed

## Compilation and Execution

1. **Compile the program:**
   ```
   g++ -std=c++17 solver.cpp -o cube_solver
   ```

2. **Run the program:**
   ```
   ./cube_solver
   ```

## Input Format

- Enter 24 characters representing the cube stickers in the following order:
  - Top (U): 4 stickers
  - Right (R): 4 stickers
  - Left (L): 4 stickers
  - Bottom (D): 4 stickers
  - Front (F): 4 stickers
  - Back (B): 4 stickers

- Use uppercase or lowercase for colors (the program converts to uppercase):
  - W (white), R (red), O (orange), Y (yellow), G (green), B (blue)

- Example input (as a single line, no spaces):
  ```
  oooobbbbyyyyggggrrrrwwww
  ```

## Output

- Prints the solution as a sequence of moves, or "Unsolvable" if the cube is not valid or cannot be solved.

  Example:
  ```
  Solution: R U2 F R'
  ```

## Files

- `solver.cpp` — C++ implementation
- `README.md` — Description and usage instructions

## Concepts Used

- Bidirectional Breadth-First Search (BFS)
- Efficient state encoding for lookup and deduplication
- Standard 2x2x2 cube move definitions
- Input validation and color counting

**Note:**  
This solver does not use permutation/orientation encoding (Lehmer code or base-3) but instead encodes the full sticker state for simplicity and clarity.
