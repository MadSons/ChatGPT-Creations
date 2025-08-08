# AGENTS.md — Multi‑Agent Guide for Building a Scalable 2D Platformer in C++

> Purpose: Direct an AI code assistant ("Codex") to plan, implement, test, and iterate on a basic-but-extensible 2D platformer using modern C++ and SDL2. Optimize for clarity, modularity, and future features. Keep everything object‑oriented but avoid unnecessary inheritance; prefer composition.

---

## 0) High-Level Product Goals

* **MVP**: Playable 2D platformer with a controllable player, camera, tilemap world, collisions, gravity/jumping, and a simple level.
* **Extensibility**: Clean abstractions for physics, rendering, input, scenes, and entities so we can later add enemies, pickups, scripting, particles, and save/load without rewrites.
* **Portability**: Windows/macOS/Linux; minimal external deps (SDL2, SDL\_image, SDL\_ttf, SDL\_mixer optional). Build with **CMake**.
* **Performance**: Fixed-time physics step; decoupled render rate; no dynamic allocations in hot loops.
* **Determinism**: Physics uses fixed dt; random sources are injectable for testability.

---

## 1) Project Structure (CMake)

```
/platformer
  ├─ CMakeLists.txt
  ├─ external/              # (optionally fetch SDL2 via CPM/FETCHContent)
  ├─ assets/
  │   ├─ tilesets/
  │   ├─ levels/
  │   └─ sprites/
  ├─ src/
  │   ├─ engine/
  │   │   ├─ Application.hpp/.cpp
  │   │   ├─ Time.hpp/.cpp               # Fixed timestep, timers
  │   │   ├─ Logger.hpp/.cpp
  │   │   ├─ Assets.hpp/.cpp             # Texture/Font/SFX managers
  │   │   ├─ Input.hpp/.cpp              # Keyboard/controller
  │   │   ├─ Renderer.hpp/.cpp           # SDL wrapper + draw primitives
  │   │   ├─ Camera.hpp/.cpp
  │   │   ├─ Math.hpp/.cpp               # Vec2, Rect, AABB, utils
  │   │   └─ Config.hpp/.cpp
  │   ├─ game/
  │   │   ├─ Scene.hpp/.cpp              # Scene base class
  │   │   ├─ SceneStack.hpp/.cpp         # Push/Pop scenes
  │   │   ├─ LevelScene.hpp/.cpp         # MVP gameplay scene
  │   │   ├─ Entity.hpp/.cpp             # Base entity (id, position, bbox)
  │   │   ├─ Components.hpp/.cpp         # Collidable, AnimatedSprite, Kinematics
  │   │   ├─ TileMap.hpp/.cpp            # CSV/JSON tile loading; collision grid
  │   │   ├─ Physics.hpp/.cpp            # Integrator, collision resolution
  │   │   ├─ Player.hpp/.cpp
  │   │   └─ UI.hpp/.cpp                 # Debug overlays, HUD text
  │   └─ main.cpp
  └─ tests/
      ├─ CMakeLists.txt
      └─ ... (unit tests)
```

**Minimal CMake** (top-level):

```cmake
cmake_minimum_required(VERSION 3.21)
project(Platformer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find or fetch SDL2 (+ image/ttf)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_subdirectory(src)
```

**src/CMakeLists.txt**:

```cmake
file(GLOB ENGINE_SRC engine/*.cpp game/*.cpp)
add_executable(platformer ${ENGINE_SRC} main.cpp)
target_include_directories(platformer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(platformer PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
```

---

## 2) Architectural Tenets

* **Composition over inheritance**: `Entity` + small components (`Kinematics`, `Collider`, `AnimatedSprite`).
* **Data locality**: Store entities in vectors where possible. Avoid shared\_ptr in core loops; use ids/handles.
* **Fixed timestep**: `Physics::step(fixed_dt)` (e.g., 1/120s), render at display rate with interpolation.
* **Separation of concerns**:

  * Input → Intent (`InputState`)
  * Physics → Integrate + resolve collisions (tile grid)
  * Rendering → Pure draw from snapshot state
  * Scenes → Game state transitions
* **Deterministic collisions**: Grid-based **AABB** vs tiles (swept or discrete with small dt), resolve axis-by-axis.

---

## 3) Core Interfaces (MVP)

### 3.1 Application / Game Loop

```cpp
class Application {
 public:
  bool init(const Config& cfg);
  void run();
  void shutdown();
 private:
  void processEvents(InputState& input);
  void fixedUpdate(float dt);   // physics
  void variableUpdate(float dt);// animations, camera smoothing
  void render(float alpha);     // alpha for interpolation
};
```

### 3.2 Scene API

```cpp
struct Scene {
  virtual ~Scene() = default;
  virtual void onEnter() {}
  virtual void onExit() {}
  virtual void handleInput(const InputState&) {}
  virtual void updateFixed(float dt) {}
  virtual void update(float dt) {}
  virtual void render(Renderer&, float alpha) const {}
};

class SceneStack { /* push/pop/peek + forward events */ };
```

### 3.3 Entity + Components (OOP-friendly composition)

```cpp
struct EntityId { uint32_t v; };

struct Transform { Vec2 pos; Vec2 size; };
struct Kinematics { Vec2 vel; Vec2 acc; bool grounded{false}; };
struct Collider { AABB bbox; bool solid{true}; };
struct AnimatedSprite { TextureId tex; IntRect src; int frame=0; float timer=0.f; };
```

Encapsulate via `Entity` façade if preferred:

```cpp
class Entity {
 public:
  Transform& transform();
  Kinematics* kinematics();
  Collider* collider();
  AnimatedSprite* sprite();
};
```

### 3.4 Tile Map & Collision Grid

```cpp
class TileMap {
 public:
  bool loadFromCSV(const std::string& path, int tileSize);
  bool isSolid(int tx, int ty) const;   // grid probe
  void draw(Renderer&) const;
  IntRect worldBounds() const;          // for camera clamp
};
```

### 3.5 Physics (AABB, axis-wise resolution)

```cpp
namespace Physics {
  void applyGravity(Kinematics&, float g, float dt);
  void integrate(Transform&, Kinematics&, float dt);
  void resolveTileCollisions(Transform&, Kinematics&, const TileMap&);
}
```

### 3.6 Camera (target follow with look‑ahead)

```cpp
class Camera {
 public:
  void setViewport(IntRect vp);
  void follow(const Transform& t, const TileMap& world, float smoothing, Vec2 lookAhead);
  IntRect view() const;
};
```

---

## 4) Input → Intent Mapping

* Raw input (SDL events) → `InputState { left,right,up,down,jump,run,pause }`.
* Player system translates `InputState` to desired horizontal accel, jump impulse, coyote time, and buffered jump.
* All rebindable via config.

---

## 5) Rendering Guidelines

* `Renderer` wraps SDL: texture creation, batching hints, camera transform.
* Draw order: background → tiles → entities → UI.
* Pixel-perfect scaling: integer scale factors preferred; camera view uses tile-sized grid.

---

## 6) Physics Rules (Platformer Basics)

* Gravity `g` constant (configurable). Fixed dt (e.g., 1/120s).
* Horizontal movement uses accel/decel with max speed and separate air control.
* Jump model: jump impulse + variable jump height (cut velocity when jump released early).
* Collision: expand AABB by velocity\*dt, resolve X then Y; set `grounded` if Y collision from above.
* Slopes/stairs: Out-of-scope for MVP, but leave hooks in `TileMap` to tag tile kinds.

---

## 7) Assets & Data

* **TileMap**: load from simple CSV (Tiled export is fine). `0` = empty, `1` = solid.
* **Sprites**: simple sprite sheet json (frame w/h, fps).
* **Fonts/UI**: SDL\_ttf for debug overlay (fps, dt, entity count).

---

## 8) Logging & Debugging

* `Logger` with levels (trace/info/warn/error) + compile-time disable.
* Debug draw toggles: collision boxes, camera rect, tile probes.
* Hotkeys: `F1` debug overlay, `R` reset level, `Esc` pause.

---

## 9) Testing Strategy

* Unit tests for: `AABB` overlap, `resolveTileCollisions`, `TileMap::isSolid`, `Camera::follow` clamping.
* Deterministic physics tests using fixed dt and seeded inputs.
* Headless build option to run tests without SDL window.

---

## 10) Coding Standards

* C++20, clang-format, warnings-as-errors (`-Wall -Wextra -Werror`), `-O2` for release.
* No RTTI/Exceptions in core loops; return `tl::expected` or status enums for recoverable errors.
* RAII for SDL resources; wrap pointers in owner types.
* Names: `PascalCase` for types, `camelCase` for methods/vars, `UPPER_SNAKE` for constants.

---

## 11) Roadmap for Future Features (Keep seams ready)

* **Enemies/AI**: Add `Actor` base and `Brain` interface; update tick per fixed step.
* **Particles**: Separate light-weight renderer system; pooled.
* **Scripting**: Optional Lua layer for level triggers and cutscenes.
* **Save/Load**: Serialize scene state and player progress (JSON/msgpack).
* **Audio**: SDL\_mixer wrapper with event hooks (jump, land, coin).
* **ECS Migration**: If needed, `Components` already isolate data to ease ECS later.

---

## 12) Agent Roles & Hand-off Contracts

### 12.1 Planner-Agent

**Inputs**: This doc, current repo state.
**Outputs**: Task graph (issues) for MVP with estimates.
**Do**:

* Create tickets for: build system, engine scaffolding, tilemap, physics, player, camera, scene stack, debug UI, tests.
* Define acceptance criteria per ticket (below).

### 12.2 Architect-Agent

**Outputs**: Header skeletons + dependency graph.
**Do**:

* Generate `.hpp` APIs first, review for cycles, minimize includes (use fwd decls + pImpl where needed).

### 12.3 Implementer-Agent

**Outputs**: `.cpp` implementations passing build + tests.
**Do**:

* Write small vertical slices (e.g., load tilemap → draw → collide) before broad stubs.

### 12.4 Tester-Agent

**Outputs**: Unit tests + a headless sim that replays inputs.
**Do**:

* Add CI steps (build both Debug/Release, run tests).

### 12.5 Refactorer-Agent

**Outputs**: Profiling notes + code cleanup PRs.
**Do**:

* Remove allocations in fixed step, pool temporary objects, flatten hot functions.

---

## 13) Acceptance Criteria (per system)

### 13.1 Build & Boot

* `cmake .. && cmake --build .` produces `platformer`.
* Window opens (800×600), clear color shown, graceful exit.

### 13.2 TileMap

* Load CSV (e.g., `assets/levels/level1.csv`), render tiles, `isSolid(tx,ty)` works.
* Query performance: ≥1M `isSolid`/sec in Release on typical desktop.

### 13.3 Physics & Collisions

* Player cannot pass through solid tiles; walking on edges stable; no falling through at 240Hz.
* Jump feels responsive: coyote time (≥100ms), jump buffer (≥100ms) implemented.

### 13.4 Camera

* Follows player, clamps to world bounds, supports look-ahead and smoothing.

### 13.5 Input

* Rebindable keys via `config.json`.

### 13.6 Debug UI

* Toggle overlay with FPS, dt, entity count, camera pos, player state.

### 13.7 Tests

* All unit tests green; deterministic physics replay matches snapshot hash.

---

## 14) Prompts & Commands for Codex

### 14.1 Bootstrap Skeleton

> **Prompt**: "Create CMake + `src/engine` + `src/game` skeleton per AGENTS.md. Generate headers first (Application, Time, Logger, Assets, Input, Renderer, Camera, Math, Config, Scene, SceneStack, LevelScene, Entity, Components, TileMap, Physics, Player, UI) with include guards and brief docs. Then add minimal `main.cpp` that opens a window and clears the screen."

### 14.2 Fixed Timestep Loop

> **Prompt**: "Implement `Application::run()` with fixed physics dt = 1/120s using accumulator. Render uses interpolation alpha. Extract SDL event handling to `processEvents`."

### 14.3 TileMap & Collisions

> **Prompt**: "Implement `TileMap` loading from CSV (',' delimited) into a vector\<uint8\_t>. Provide `isSolid(tx,ty)`. Implement `Physics::resolveTileCollisions` using axis-separated resolution on grid tiles for an AABB."

### 14.4 Player Controller

> **Prompt**: "Implement `Player` using `Kinematics` with accel/decel, air control, coyote time, jump buffer, variable jump. Expose tunables in `Config`."

### 14.5 Camera

> **Prompt**: "Implement `Camera::follow` with smooth damp towards target and optional look-ahead based on player velocity. Clamp to `TileMap::worldBounds()`."

### 14.6 Debug Overlay

> **Prompt**: "Add UI overlay with SDL\_ttf showing fps, dt, player pos/vel, grounded flag; toggle with F1."

### 14.7 Tests

> **Prompt**: "Add tests for AABB overlap, tile queries, and collision resolution (X,Y cases). Provide a headless build option `-DHEADLESS=ON`."

---

## 15) Config Defaults (tweakable)

```json
{
  "window": {"width": 800, "height": 600, "title": "Platformer"},
  "physics": {"fixed_dt": 0.0083333, "gravity": 2200.0},
  "player": {
    "move_accel": 18000.0,
    "move_decel": 26000.0,
    "max_speed": 260.0,
    "air_control": 0.6,
    "jump_impulse": 760.0,
    "coyote_time": 0.12,
    "jump_buffer": 0.12
  },
  "input": {
    "left": "A", "right": "D", "up": "W", "down": "S", "jump": "SPACE", "run": "LCTRL"
  }
}
```

---

## 16) Definition of Done (MVP)

* CI builds for Win/macOS/Linux; app runs; can load a level; move/jump with predictable feel; camera smooth; collisions stable; debug overlay works; tests pass.

---

## 17) Known Non-Goals (for now)

* Network play, slopes/one-way platforms, complex AI/pathfinding, fancy shaders, ECS conversion. Design leaves seams for these later.

---

## 18) Notes for Future Contributors

* Keep `.hpp` minimal; include what you use; prefer forward declarations.
* Public APIs are stable; internal details may change. Document invariants.
* If you add a new system, add a test.
