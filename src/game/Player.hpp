#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Components.hpp"

// Controllable player entity.
struct Player : public Entity {
    Kinematics kinematics;
    bool grounded{false};
    bool canDoubleJump{true};
    float width{32.0f};
    float height{32.0f};
};

#endif // PLAYER_HPP
