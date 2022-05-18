#ifndef TEXTSPRITE_H
#define TEXTSPRITE_H

#include <string>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "Render/TextSpriteTask.h"

class TextSprite final : public GameObject {
private:
    TextSpriteTask task;

public:
    explicit TextSprite(std::string name) : task(*this) { Name = std::move(name); }

    [[nodiscard]] IRenderTask *GetRenderTask() final
    {
        task = *this;
        return &task;
    }

    glm::vec3 Color = glm::vec3(0.0f, 0.0f, 0.0f);
    std::string Text;
};

#endif
