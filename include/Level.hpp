#pragma once
#include <SDL2/SDL.h>

class Level {
public:
    Level(int width, int height, int groundY);
    void render(SDL_Renderer* renderer, float cameraX) const;
    int groundY() const { return groundY_; }
private:
    int width_;
    int height_;
    int groundY_;
    static const int TILE_SIZE = 40;
};
