#include "Input.hpp"
#include <SDL.h>

void Input::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        bool pressed = e.type == SDL_KEYDOWN;
        switch (e.key.keysym.sym) {
            case SDLK_a: left = pressed; break;
            case SDLK_s: right = pressed; break;
            case SDLK_SPACE: jump = pressed; break;
        }
    }
}

void Input::update() {
    jumpPressed = jump && !m_prevJump;
    m_prevJump = jump;
}

