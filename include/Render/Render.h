#ifndef RENDER_H
#define RENDER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"
#include "Game/Sprite.h"
#include "Game/TextSprite.h"
#include "Game/Camera.h"

class Render {
public:
    Render(int width, int height);
    ~Render();

    void RenderSprite(Sprite *sprite) const;
    void RenderTextSprite(TextSprite *sprite) const;

    void ResizeWindow(int width, int height);

    glm::mat4 ProjectMat = glm::mat4(1.f);
    float Zoom = 1.0f;

private:
    unsigned int boxVAO{};
};

#endif
