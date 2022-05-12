#ifndef TEXTSPRITE_H
#define TEXTSPRITE_H

#include <string>
#include <glm/glm.hpp>
#include "GameObject.h"

class TextSprite final : public GameObject {
public:
    explicit TextSprite(std::string name) { Name = std::move(name); }

    glm::vec3 Color = glm::vec3(0.0f, 0.0f, 0.0f);
    std::string Text;
};

#endif
