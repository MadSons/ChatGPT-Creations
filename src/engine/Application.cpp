#include "Application.hpp"
#include "Config.hpp"
#include "Input.hpp"
#include "game/Player.hpp"
#include "game/TileMap.hpp"
#include "game/Physics.hpp"

#include <SDL.h>

bool Application::init(const Config& cfg) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    m_window = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, cfg.width, cfg.height, 0);
    if (!m_window) return false;
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) return false;

    // load level
    if (!m_map.loadFromCSV("assets/levels/level1.csv")) return false;

    m_camera = Camera(cfg.width, cfg.height);
    m_player.position = {64.0f, 64.0f};
    m_running = true;
    return true;
}

void Application::run() {
    const float dt = 1.0f / 60.0f;
    Uint32 prev = SDL_GetTicks();
    float accumulator = 0.0f;
    while (m_running) {
        Uint32 current = SDL_GetTicks();
        float frameTime = (current - prev) / 1000.0f;
        if (frameTime > 0.25f) frameTime = 0.25f;
        prev = current;
        accumulator += frameTime;

        processEvents();

        while (accumulator >= dt) {
            fixedUpdate(dt);
            accumulator -= dt;
        }

        render(0.0f);
    }
}

void Application::shutdown() {
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window) SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Application::processEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) m_running = false;
        m_input.handleEvent(e);
    }
    m_input.update();
}

void Application::fixedUpdate(float dt) {
    const float baseSpeed = 200.0f;
    const float moveSpeed = m_input.run ? baseSpeed * 1.75f : baseSpeed;
    if (m_input.left)
        m_player.kinematics.vx = -moveSpeed;
    else if (m_input.right)
        m_player.kinematics.vx = moveSpeed;
    else
        m_player.kinematics.vx = 0.0f;

    if (m_input.jumpPressed) {
        if (m_player.grounded) {
            m_player.kinematics.vy = -550.0f;
            m_player.grounded = false;
        } else if (m_player.canDoubleJump) {
            m_player.kinematics.vy = -550.0f;
            m_player.canDoubleJump = false;
        }
    }

    m_physics.step(m_player, m_map, dt);
    if (m_player.grounded) m_player.canDoubleJump = true;

    constexpr int tileSize = 32;
    m_camera.follow(m_player.position, m_map.width() * tileSize, m_map.height() * tileSize);
}

void Application::variableUpdate(float /*dt*/) {}

void Application::render(float /*alpha*/) {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    // draw tiles
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    const int tileSize = 32;
    int camX = static_cast<int>(m_camera.position().x);
    int camY = static_cast<int>(m_camera.position().y);
    int startX = camX / tileSize;
    int startY = camY / tileSize;
    int endX = (camX + m_camera.width() + tileSize - 1) / tileSize;
    int endY = (camY + m_camera.height() + tileSize - 1) / tileSize;
    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            if (m_map.isSolid(x, y)) {
                SDL_Rect r{ x * tileSize - camX, y * tileSize - camY, tileSize, tileSize };
                SDL_RenderFillRect(m_renderer, &r);
            }
        }
    }

    // draw player
    SDL_SetRenderDrawColor(m_renderer, 200, 0, 0, 255);
    SDL_Rect pr{ static_cast<int>(m_player.position.x - camX), static_cast<int>(m_player.position.y - camY),
                 static_cast<int>(m_player.width), static_cast<int>(m_player.height) };
    SDL_RenderFillRect(m_renderer, &pr);

    SDL_RenderPresent(m_renderer);
}
