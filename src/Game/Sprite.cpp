#include "Game/Sprite.h"

Sprite::Sprite(std::string name) :
        task(*this)
{
    Name = std::move(name);
    Shader = "Standard";
}

IRenderTask *Sprite::GetRenderTask()
{
    task = SpriteTask(*this);
    return &task;
}
