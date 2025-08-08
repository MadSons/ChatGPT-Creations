#include "Player.hpp"

Player::Player()
    : x(100.0f), y(0.0f), vx(0.0f), vy(0.0f), w(50), h(50) {}

void Player::handleEvent(const SDL_Event& e) {
    const float moveSpeed = 5.0f;
    const float jumpSpeed = 12.0f;
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_a: vx = -moveSpeed; break;
            case SDLK_d: vx = moveSpeed; break;
            case SDLK_SPACE:
                if (vy == 0.0f) vy = -jumpSpeed;
                break;
        }
    } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_a:
                if (vx < 0.0f) vx = 0.0f;
                break;
            case SDLK_d:
                if (vx > 0.0f) vx = 0.0f;
                break;
        }
    }
}

void Player::update(float gravity, float groundY) {
    vy += gravity;
    x += vx;
    y += vy;
    if (y + h >= groundY) {
        y = groundY - h;
        vy = 0.0f;
    }
}

void Player::render(SDL_Renderer* renderer, float cameraX) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect{static_cast<int>(x - cameraX), static_cast<int>(y), w, h};
    SDL_RenderFillRect(renderer, &rect);
}

float Player::getX() const { return x; }
float Player::getY() const { return y; }
int Player::getWidth() const { return w; }
int Player::getHeight() const { return h; }
