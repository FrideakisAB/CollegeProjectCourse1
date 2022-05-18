#ifndef EXTSPRITETASK_H
#define EXTSPRITETASK_H

#include "Render/IRenderTask.h"

class TextSprite;

class TextSpriteTask final : public IRenderTask {
private:
    glm::vec3 color;
    std::string_view text;

public:
    TextSpriteTask(const TextSprite &sprite);
    ~TextSpriteTask() final = default;

    void UIDraw() final;
};

#endif
