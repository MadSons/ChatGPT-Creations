# C++ Platformer Prototype

This repository begins a simple 2D platformer built from scratch in C++ using SDL2. The initial demo opens a window with a checkerboard background and a controllable rectangle character standing on a ground line. The camera follows the player to create a scrolling effect.

## Directory Structure
- `src/` – C++ source files.
- `include/` – header files and utilities.
- `assets/`
  - `sprites/` – character and object sprites.
  - `backgrounds/` – background images and tiles.
- `CMakeLists.txt` – build configuration using CMake.
- `.gitignore` – ignores build artifacts and binaries.

## Build Instructions
```
mkdir build && cd build
cmake ..
make
./platformer
```

## Controls
- **A** – move left
- **D** – move right
- **Space** – jump

Compile with `-DDEBUG` to enable simple logging via the `LOG` macro.
