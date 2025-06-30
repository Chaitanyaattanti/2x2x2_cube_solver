# 2x2x2 Rubik's Cube Solver in C++

This project implements a solver for the 2x2x2 Rubik’s Cube (Pocket Cube) using Breadth-First Search (BFS). The cube's state is efficiently encoded using permutation and orientation to reduce the search space and solve efficiently.

## Features

- Solves any valid 2x2x2 cube configuration
- Finds the shortest sequence of moves using BFS
- Compact cube state representation using permutation and orientation
- Supports 9 moves: R, R', R2, U, U', U2, F, F', F2
- Automatically generates the target state by flipping all face colors

## How It Works

- Each corner of the cube is represented by its position and orientation
- The cube state is encoded using Lehmer code and base-3 encoding
- The goal state is computed by inverting face colors
- BFS explores the shortest path from start to goal
- Once the solution is found, the move sequence is printed

## Compilation and Execution

1. Compile the program:

   ```
   g++ -std=c++17 solver.cpp -o cube_solver
   ```

2. Run the program:

   ```
   ./cube_solver
   ```

## Input Format

- Enter 6 faces of the cube, each with 4 stickers (2 rows of 2)
- Input 24 lowercase letters representing cube face colors:
  - `r` (red), `o` (orange), `w` (white), `y` (yellow), `b` (blue), `g` (green)

- Example input:

  ```
  o o o o
  b b b b
  y y y y
  g g g g
  r r r r
  w w w w
  ```

## Output

- Shows the initial and goal cube configuration
- Prints the move sequence that solves the cube

  Example:

  ```
  Moves:
  R U2 F R'
  ```

## Files

- `solver.cpp` — C++ implementation
- `README.md` — Description and usage instructions

## Concepts Used

- Breadth-First Search (BFS)
- Permutation encoding using Lehmer code
- Orientation encoding with base-3 system
- Efficient state representation and transition
