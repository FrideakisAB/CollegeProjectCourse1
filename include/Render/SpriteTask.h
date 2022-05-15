#ifndef SPRITETASK_H
#define SPRITETASK_H

#include "Render/IRenderTask.h"

class Sprite;

class SpriteTask final : public IRenderTask {
private:
    static inline unsigned int boxVAO = 0;

    static void initializeMesh();

public:
    SpriteTask(const Sprite &sprite);
    ~SpriteTask() final = default;

    void Draw(const Shader &shader) override;

    std::string_view Texture;
};

#endif
