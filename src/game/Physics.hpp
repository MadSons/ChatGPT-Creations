#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "Player.hpp"
#include "TileMap.hpp"

// Physics system handling collisions.
class Physics {
public:
    void step(Player& p, const TileMap& map, float dt);
    float gravity{2000.0f};
};

#endif // PHYSICS_HPP
