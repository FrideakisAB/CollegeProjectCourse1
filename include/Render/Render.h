#ifndef RENDER_H
#define RENDER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"
#include "Game/Sprite.h"
#include "Game/TextSprite.h"
#include "Game/Camera.h"
#include "Game/LinesSprite.h"

class Render {
public:
    Render(int width, int height);
    ~Render();

    void RenderSprite(const Sprite &sprite) const;
    void RenderTextSprite(const TextSprite &sprite) const;
    void RenderLinesSprite(const LinesSprite &sprite) const;

    void ResizeWindow(int width, int height);

    glm::mat4 ProjectMat = glm::mat4(1.f);
    float Zoom = 1.0f;

    [[nodiscard]] glm::ivec2 GetWindowSize() const noexcept { return {width, height}; }

private:
    int width, height;
    unsigned int boxVAO{};
    unsigned int linesVAO{}, linesVBO{};
};

#endif
