#ifndef TEXTSPRITE_H
#define TEXTSPRITE_H

#include <string>
#include "Sprite.h"
#include <glm/glm.hpp>

class TextSprite
{
public:
    float x, y, z, sx, sy;

    TextSprite(std::string name, std::string tg = "");
    virtual ~TextSprite();

    void setText(std::string txt);
    std::string getText();

    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

private:
    std::string txt = "";
};

#endif // TEXTSPRITE_H
