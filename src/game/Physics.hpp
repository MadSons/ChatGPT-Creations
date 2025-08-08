#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "Entity.hpp"
#include "TileMap.hpp"

// Physics system handling collisions.
class Physics {
public:
    void step(Entity& e, const TileMap& map, float dt);
};

#endif // PHYSICS_HPP
