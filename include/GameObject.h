#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <glm/glm.hpp>

class IRenderTask;

class GameObject {
public:
    virtual ~GameObject() = default;

    [[nodiscard]] virtual IRenderTask *GetRenderTask() { return nullptr; }

    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec2 Scale = glm::vec2(1.0f);

    std::string Name;
};

#endif
