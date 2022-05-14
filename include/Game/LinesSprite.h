#ifndef LINESSPRITE_H
#define LINESSPRITE_H

#include <string>
#include <vector>
#include "GameObject.h"

class LinesSprite final : public GameObject {
private:
    std::vector<glm::vec2> Points{};

public:
    explicit LinesSprite(std::string name) { Name = std::move(name); }

    void AddPoint(glm::vec2 point);
    void ClearPoints();
    [[nodiscard]] const glm::vec2 *GetPointsPtr() const noexcept;

    int PointsPosition = 0;
    bool IsUpdate = false;
};

#endif
