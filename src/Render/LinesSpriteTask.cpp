#include "Render/LinesSpriteTask.h"

#include "Game/LinesSprite.h"

LinesSpriteTask::LinesSpriteTask(const LinesSprite &sprite)
{
    ShaderName = "Lines";
    pointsPosition = sprite.PointsPosition;
    pointsPtr = sprite.GetPointsPtr();
    isUpdate = sprite.IsUpdate;

    Position = sprite.Position;
    Scale = sprite.Scale;
}

void LinesSpriteTask::initializeMesh()
{
    glGenVertexArrays(1, &linesVAO);
    glGenBuffers(1, &linesVBO);

    glBindVertexArray(linesVAO);

    glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec2), nullptr, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void LinesSpriteTask::updateMesh()
{
    glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
    glBufferData(GL_ARRAY_BUFFER, pointsPosition * sizeof(glm::vec2), pointsPtr, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LinesSpriteTask::Draw(const Shader &shader)
{
    if (linesVAO == 0)
        initializeMesh();

    if (pointsPosition > 1)
    {
        if (isUpdate)
            updateMesh();

        glBindVertexArray(linesVAO);
        glDrawArrays(GL_LINE_STRIP, 0, pointsPosition);
        glBindVertexArray(0);
    }
}

void LinesSpriteTask::Update(const LinesSprite &sprite)
{
    pointsPosition = sprite.PointsPosition;
    pointsPtr = sprite.GetPointsPtr();
    isUpdate = sprite.IsUpdate;
}
