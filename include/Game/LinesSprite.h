#ifndef LINESSPRITE_H
#define LINESSPRITE_H

#include <string>
#include <vector>
#include "GameObject.h"

class LinesSprite final : public GameObject {
public:
    explicit LinesSprite(std::string name) { Name = std::move(name); }

    std::vector<glm::vec2> Points{};
    int PointsPosition = 0;
};

#endif
