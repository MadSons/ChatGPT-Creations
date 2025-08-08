#include <SDL2/SDL.h>
#include "debug.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int TILE_SIZE = 40;
const int GROUND_Y = WINDOW_HEIGHT - 50; // top of the ground line

struct Player {
    float x{100};
    float y{GROUND_Y - 50};
    float vx{0};
    float vy{0};
    int w{50};
    int h{50};
};

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Platformer Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    Player player;
    float cameraX = 0.0f;
    const float gravity = 0.5f;
    const float moveSpeed = 5.0f;
    const float jumpSpeed = 12.0f;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_a: player.vx = -moveSpeed; break;
                    case SDLK_d: player.vx = moveSpeed; break;
                    case SDLK_SPACE:
                        if (player.y + player.h >= GROUND_Y) {
                            player.vy = -jumpSpeed;
                        }
                        break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_a:
                        if (player.vx < 0) player.vx = 0;
                        break;
                    case SDLK_d:
                        if (player.vx > 0) player.vx = 0;
                        break;
                }
            }
        }

        // physics
        player.vy += gravity;
        player.x += player.vx;
        player.y += player.vy;
        if (player.y + player.h >= GROUND_Y) {
            player.y = GROUND_Y - player.h;
            player.vy = 0;
        }

        // camera follows player
        cameraX = player.x - WINDOW_WIDTH / 2;
        if (cameraX < 0) cameraX = 0;

        // render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // checkerboard background
        for (int y = 0; y < WINDOW_HEIGHT / TILE_SIZE + 1; ++y) {
            for (int x = 0; x < WINDOW_WIDTH / TILE_SIZE + 2; ++x) {
                int worldX = x * TILE_SIZE + ((int)cameraX % TILE_SIZE) - TILE_SIZE;
                int worldY = y * TILE_SIZE;
                bool dark = (( ( (worldX + cameraX) / TILE_SIZE) + y) % 2) == 0;
                if (dark) SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                else SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_Rect tileRect { worldX - (int)cameraX, worldY, TILE_SIZE, TILE_SIZE };
                SDL_RenderFillRect(renderer, &tileRect);
            }
        }

        // ground line
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawLine(renderer, -cameraX, GROUND_Y, WINDOW_WIDTH - cameraX, GROUND_Y);

        // player
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect playerRect { (int)(player.x - cameraX), (int)player.y, player.w, player.h };
        SDL_RenderFillRect(renderer, &playerRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);

        LOG("player: " << player.x << "," << player.y << " camera: " << cameraX);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
