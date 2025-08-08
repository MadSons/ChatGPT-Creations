#include "Level.hpp"

Level::Level(int width, int height, int groundY)
    : width_(width), height_(height), groundY_(groundY) {}

void Level::render(SDL_Renderer* renderer, float cameraX) const {
    for (int y = 0; y < height_ / TILE_SIZE + 1; ++y) {
        for (int x = 0; x < width_ / TILE_SIZE + 2; ++x) {
            int worldX = x * TILE_SIZE + (static_cast<int>(cameraX) % TILE_SIZE) - TILE_SIZE;
            int worldY = y * TILE_SIZE;
            bool dark = ((((worldX + static_cast<int>(cameraX)) / TILE_SIZE) + y) % 2) == 0;
            if (dark) SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            else SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_Rect tileRect{worldX - static_cast<int>(cameraX), worldY, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tileRect);
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, -cameraX, groundY_, width_ - cameraX, groundY_);
}
