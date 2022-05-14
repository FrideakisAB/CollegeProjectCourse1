#include "Game/LinesSprite.h"

void LinesSprite::AddPoint(glm::vec2 point)
{
    Points.push_back(point);
    IsUpdate = true;
}

void LinesSprite::ClearPoints()
{
    Points.clear();
    PointsPosition = 0;
}

const glm::vec2 *LinesSprite::GetPointsPtr() const noexcept
{
    return Points.data();
}
