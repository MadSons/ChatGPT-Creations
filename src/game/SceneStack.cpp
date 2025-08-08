#include "SceneStack.hpp"

void SceneStack::push(Scene* scene) { m_stack.push_back(scene); }
void SceneStack::pop() { if(!m_stack.empty()) m_stack.pop_back(); }
