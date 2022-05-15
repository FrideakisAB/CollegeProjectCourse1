#ifndef IRENDERTASK_H
#define IRENDERTASK_H

#include <string>
#include "Render/Shader.h"

class IRenderTask {
public:
    virtual ~IRenderTask() = default;

    virtual void Draw(const Shader &shader) = 0;

    std::string_view ShaderName{};
    bool IsOverUIRenderMode = false;
    glm::vec3 Position{};
    glm::vec2 Scale{1.f};
};

#endif
