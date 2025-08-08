#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Components.hpp"

// Controllable player entity.
struct Player : public Entity {
    Kinematics kinematics;
};

#endif // PLAYER_HPP
