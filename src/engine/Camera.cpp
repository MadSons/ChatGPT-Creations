#include "Camera.hpp"
#include <algorithm>

Camera::Camera(int width, int height) : m_width(width), m_height(height) {}

void Camera::follow(const Vec2& target, int levelWidth, int levelHeight) {
    float halfW = static_cast<float>(m_width) * 0.5f;
    float halfH = static_cast<float>(m_height) * 0.5f;
    float desiredX = target.x - halfW;
    float desiredY = target.y - halfH;
    float maxX = static_cast<float>(levelWidth - m_width);
    float maxY = static_cast<float>(levelHeight - m_height);
    if (maxX < 0.f) maxX = 0.f;
    if (maxY < 0.f) maxY = 0.f;
    m_pos.x = std::clamp(desiredX, 0.0f, maxX);
    m_pos.y = std::clamp(desiredY, 0.0f, maxY);
}
