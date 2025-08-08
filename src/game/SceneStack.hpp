#ifndef SCENESTACK_HPP
#define SCENESTACK_HPP

#include <vector>

class Scene;

// Manages a stack of scenes.
class SceneStack {
public:
    void push(Scene* scene);
    void pop();
private:
    std::vector<Scene*> m_stack;
};

#endif // SCENESTACK_HPP
