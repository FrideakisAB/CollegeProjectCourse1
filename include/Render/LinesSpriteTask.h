#ifndef LINESSPRITETASK_H
#define LINESSPRITETASK_H

#include "Render/IRenderTask.h"

class LinesSprite;

class LinesSpriteTask final : public IRenderTask {
private:
    unsigned int linesVAO{}, linesVBO{};

    int pointsPosition{};
    const glm::vec2 *pointsPtr{};
    bool isUpdate = false;

    void initializeMesh();
    void updateMesh();

public:
    LinesSpriteTask(const LinesSprite &sprite);
    ~LinesSpriteTask() final = default;

    void Draw(const Shader &shader) final;
    void Update(const LinesSprite &sprite);
};

#endif
