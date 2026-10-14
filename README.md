# Snake in C

A classic Snake game that runs in the Windows terminal, written from scratch in C with no game libraries. I built it to practise core C concepts: structs, arrays, pointers, functions and a real-time game loop.

## Features

- Snake that moves continuously and is steered with the keyboard
- Random food placement that never spawns on the snake or the walls
- Snake grows when it eats, and the score goes up
- Game over on hitting a wall or the snake's own body
- Smooth redraw with no flicker (the cursor is repositioned instead of clearing the screen)

## Controls

| Key | Action |
|---|---|
| `W` | Move up |
| `A` | Move left |
| `S` | Move down |
| `D` | Move right |
| `Q` | Quit |

The snake cannot reverse straight into itself, so pressing the opposite direction is ignored.

## Requirements

- **Windows** (the game uses `windows.h` and `conio.h`)
- **GCC** (for example MinGW-w64)

## How to run

Run it from a terminal such as PowerShell, Command Prompt or the VS Code terminal. If you double-click the `.exe`, the window may close quickly. Make the terminal window tall enough to show the whole board (about 23 lines).

### Data design

The snake is an **array of structs**. Each struct is a `Point` holding one cell's `x` and `y`. Everything about the snake is bundled into a second struct:

```c
typedef struct { int x, y; } Point;

typedef struct {
    Point body[MAX_LEN];   // body[0] is the head
    int length, dx, dy;    // dx, dy = current direction
} Snake;
```

The array holds `MAX_LEN` slots, but only the first `length` are in use.

### The game loop

Each frame runs these steps in order:

1. Reset the cursor to the top-left of the console
2. Read input (non-blocking)
3. Move the snake
4. Check for collisions
5. Check whether the food was eaten
6. Draw the board
7. Sleep for 100 ms

### Movement

Only the head is steered. Each body segment simply copies the position of the segment in front of it. The update runs from the **tail toward the head**, so every segment copies a position that has not yet been overwritten. Going head-first would stack the whole snake on one cell. Then the head moves by `(dx, dy)`.

### Drawing

The board is printed once per frame in a single pair of nested loops. For every cell the program decides what to print, in this order: border, head, body, food, empty space. The body check is a small search through the snake array.

### Growth

Before each move, the tail's position is saved. If the head lands on the food after the move, the saved position becomes a new last segment, so the snake grows from the back.

### Collisions

Only the head can cause a crash. It is compared against the wall cells, then against body segments 1 to `length - 1`.

### Non-blocking input

`_kbhit()` checks whether a key is waiting, and `_getch()` reads it without needing Enter. If no key is pressed, the snake just keeps its direction.

## C concepts practised

- `struct`, `typedef` and an array of structs
- Pointers: functions take `Snake *` and `Point *` so they work on the real data instead of copies, with `->` for pointer access
- `#define` constants and why a define with an expression needs parentheses
- Function prototypes and splitting a program into small single-purpose functions
- Loops, `break`, `switch` and boolean flags
- Array bounds: loops run to `length`, never to `MAX_LEN`, and growth is guarded against overflowing the array
- Random numbers with `rand()` and `srand(time(NULL))`
- Windows console API: `SetConsoleCursorPosition`, `SetConsoleCursorInfo`, `Sleep`

## Project structure

```
.
├── snake.c
└── README.md
```

## Author

Built by **Rivu Ghosh** as a first-year engineering student learning C.
