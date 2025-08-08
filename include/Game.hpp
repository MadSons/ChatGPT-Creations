#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Player.hpp"
#include "Level.hpp"

class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();
private:
    void processInput();
    void update();
    void render();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    std::vector<Level> levels;
    size_t currentLevel;
    Player player;
    float cameraX;
};
