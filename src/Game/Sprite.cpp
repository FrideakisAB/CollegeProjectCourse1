#include "Game/Sprite.h"

Sprite::Sprite(std::string name)
{
    Name = std::move(name);
    Shader = "Standard";
}
