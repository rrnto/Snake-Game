# Snake Game in C (SDL2)

This folder contains a Snake game implemented in C using SDL2.

## Features

- Graphical snake game with grid-based movement
- Collision detection with walls and snake body
- Clickable "Restart" button after game over
- Adjustable speed and grid size

## Requirements

- SDL2 installed (headers in `include/`, libraries in `lib/`, DLL in `bin/`)
- MinGW or GCC compiler on Windows

## How to Play

- Go to the `bin/` folder and double-click `snake.exe` or `snake2.exe` to play directly.
- Arrow keys control the snake.
- Click the green "Restart" button to play again after game over.

## Modifying the Code

- `src/main.c` contains the first test version.
- `src/main2.c` contains the final version with full features.
- You can edit these files and recompile using the example below.

## Compilation Example

```bash
gcc src/main2.c -o bin/snake2.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2

