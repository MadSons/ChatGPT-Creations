#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Math.hpp"

// Basic camera that follows a target within world bounds.
class Camera {
public:
    Camera(int width, int height);

    // Center camera on target while clamping to level bounds.
    void follow(const Vec2& target, int levelWidth, int levelHeight);

    const Vec2& position() const { return m_pos; }
    int width() const { return m_width; }
    int height() const { return m_height; }

private:
    Vec2 m_pos{};
    int m_width{0};
    int m_height{0};
};

#endif // CAMERA_HPP
