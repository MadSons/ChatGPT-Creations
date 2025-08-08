#include "Physics.hpp"

namespace {
constexpr int TILE_SIZE = 32;
}

void Physics::step(Player& p, const TileMap& map, float dt) {
    // apply gravity
    p.kinematics.vy += gravity * dt;

    // horizontal movement
    p.position.x += p.kinematics.vx * dt;
    if (p.kinematics.vx > 0) {
        int tx = static_cast<int>((p.position.x + p.width) / TILE_SIZE);
        int top = static_cast<int>(p.position.y / TILE_SIZE);
        int bottom = static_cast<int>((p.position.y + p.height - 1) / TILE_SIZE);
        for (int ty = top; ty <= bottom; ++ty) {
            if (map.isSolid(tx, ty)) {
                p.position.x = tx * TILE_SIZE - p.width;
                p.kinematics.vx = 0;
                break;
            }
        }
    } else if (p.kinematics.vx < 0) {
        int tx = static_cast<int>(p.position.x / TILE_SIZE);
        int top = static_cast<int>(p.position.y / TILE_SIZE);
        int bottom = static_cast<int>((p.position.y + p.height - 1) / TILE_SIZE);
        for (int ty = top; ty <= bottom; ++ty) {
            if (map.isSolid(tx, ty)) {
                p.position.x = (tx + 1) * TILE_SIZE;
                p.kinematics.vx = 0;
                break;
            }
        }
    }

    // vertical movement
    p.position.y += p.kinematics.vy * dt;
    p.grounded = false;
    if (p.kinematics.vy > 0) {
        int ty = static_cast<int>((p.position.y + p.height) / TILE_SIZE);
        int left = static_cast<int>(p.position.x / TILE_SIZE);
        int right = static_cast<int>((p.position.x + p.width - 1) / TILE_SIZE);
        for (int tx = left; tx <= right; ++tx) {
            if (map.isSolid(tx, ty)) {
                p.position.y = ty * TILE_SIZE - p.height;
                p.kinematics.vy = 0;
                p.grounded = true;
                break;
            }
        }
    } else if (p.kinematics.vy < 0) {
        int ty = static_cast<int>(p.position.y / TILE_SIZE);
        int left = static_cast<int>(p.position.x / TILE_SIZE);
        int right = static_cast<int>((p.position.x + p.width - 1) / TILE_SIZE);
        for (int tx = left; tx <= right; ++tx) {
            if (map.isSolid(tx, ty)) {
                p.position.y = (ty + 1) * TILE_SIZE;
                p.kinematics.vy = 0;
                break;
            }
        }
    }
}
