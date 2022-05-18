#ifndef IRENDERTASK_H
#define IRENDERTASK_H

#include <string>
#include "Render/Shader.h"

class IRenderTask {
public:
    virtual ~IRenderTask() = default;

    virtual void Draw(const Shader &shader) {}
    virtual void UIDraw() {}

    std::string_view ShaderName{};
    glm::vec3 Position{};
    glm::vec2 Scale{1.f};
};

#endif
