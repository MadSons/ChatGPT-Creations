#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include "Stats.hpp"
#include "Ability.hpp"
#include "Item.hpp"

class Player {
public:
    Player();
    void handleEvent(const SDL_Event& e);
    void update(float gravity, float groundY);
    void render(SDL_Renderer* renderer, float cameraX) const;
    float getX() const;
    float getY() const;
    int getWidth() const;
    int getHeight() const;
private:
    float x;
    float y;
    float vx;
    float vy;
    int w;
    int h;
    Stats stats;
    std::vector<std::shared_ptr<Ability>> abilities;
    std::vector<std::shared_ptr<Item>> inventory;
};
