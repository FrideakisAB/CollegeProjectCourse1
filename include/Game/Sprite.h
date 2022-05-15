#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include "GameObject.h"
#include "Render/SpriteTask.h"

class Sprite final : public GameObject {
private:
    SpriteTask task;

public:
    explicit Sprite(std::string name);

    [[nodiscard]] IRenderTask *GetRenderTask() override;

    std::string Texture, Shader;
};

#endif
