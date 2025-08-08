# C++ Platformer Prototype

This repository provides a scaffold for a 2D platformer built in modern C++ using SDL2. The demo now features a controllable player, scrolling level, and basic physics.

## Directory Structure
- `src/`
  - `engine/` – core engine systems (application, renderer, etc.).
  - `game/` – gameplay code such as scenes, entities, and physics.
  - `CMakeLists.txt` – build rules for the executable.
- `assets/`
    - `tilesets/` – tile graphics.
    - `levels/` – level data (`level1.csv` now spans multiple screens and scrolls with the camera).
    - `sprites/` – character and object sprites.
- `external/` – optional third‑party dependencies.
- `tests/` – placeholder for unit tests.
- `CMakeLists.txt` – top‑level build configuration.

## Build Instructions
Ensure SDL2 is installed on your system. Image and TTF support are optional and will be enabled automatically if the development packages are available. On Debian/Ubuntu:

```
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

Then build the demo:

```
cmake -S . -B build
cmake --build build
./build/src/platformer
```

The executable is placed under `build/src/`. Running it creates an 800×600 window with a side‑scrolling level. Use **A/D** to move, **SPACE** to jump (with a double jump), and hold **Left Shift** to sprint.
