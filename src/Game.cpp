#include "Game.hpp"
#include "debug.hpp"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;

Game::Game()
    : window(nullptr), renderer(nullptr), running(false),
      levels(), currentLevel(0), player(), cameraX(0.0f) {}

Game::~Game() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("Platformer Demo", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    levels.emplace_back(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_HEIGHT - 50);
    running = true;
    return true;
}

void Game::run() {
    while (running) {
        processInput();
        update();
        render();
        SDL_Delay(16);
        LOG("player: " << player.getX() << "," << player.getY() << " camera: " << cameraX);
    }
}

void Game::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        player.handleEvent(event);
    }
}

void Game::update() {
    const float gravity = 0.5f;
    player.update(gravity, levels[currentLevel].groundY());
    cameraX = player.getX() - WINDOW_WIDTH / 2;
    if (cameraX < 0) cameraX = 0;
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    levels[currentLevel].render(renderer, cameraX);
    player.render(renderer, cameraX);

    SDL_RenderPresent(renderer);
}
