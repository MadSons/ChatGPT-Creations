#ifndef APPLICATION_HPP
#define APPLICATION_HPP

class Config;
struct SDL_Window;
struct SDL_Renderer;

#include "Input.hpp"
#include "game/Player.hpp"
#include "game/TileMap.hpp"
#include "game/Physics.hpp"

// Main application managing the game loop.
class Application {
public:
    bool init(const Config& cfg);
    void run();
    void shutdown();

private:
    void processEvents();
    void fixedUpdate(float dt);
    void variableUpdate(float dt);
    void render(float alpha);

    SDL_Window* m_window{nullptr};
    SDL_Renderer* m_renderer{nullptr};
    bool m_running{false};

    Input m_input;
    Player m_player;
    TileMap m_map;
    Physics m_physics;
};

#endif // APPLICATION_HPP
