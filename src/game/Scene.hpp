#ifndef SCENE_HPP
#define SCENE_HPP

// Base class for game scenes.
class Scene {
public:
    virtual ~Scene() = default;
    virtual void update(float dt) = 0;
};

#endif // SCENE_HPP
