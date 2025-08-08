#include "Input.hpp"
#include <SDL.h>

void Input::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        bool pressed = e.type == SDL_KEYDOWN;
        switch (e.key.keysym.sym) {
            case SDLK_a: left = pressed; break;
            case SDLK_d: right = pressed; break;
            case SDLK_SPACE:
                jump = pressed;
                if (pressed && e.key.repeat == 0) m_jumpEvent = true;
                break;
            case SDLK_LSHIFT: run = pressed; break;
        }
    }
}

void Input::update() {
    jumpPressed = (jump && !m_prevJump) || m_jumpEvent;
    m_prevJump = jump;
    m_jumpEvent = false;
}

