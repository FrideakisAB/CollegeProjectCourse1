#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include "GameObject.h"

class Sprite final : public GameObject {
public:
    explicit Sprite(std::string name);
    ~Sprite() final = default;

    std::string Texture, Shader;
};

#endif
