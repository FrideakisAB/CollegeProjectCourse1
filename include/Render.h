#ifndef RENDER_H
#define RENDER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TextureManager.h"
#include "Sprite.h"
#include "TextSprite.h"
#include "Camera.h"

class Render
{
public:
    Render(int width, int height, TextureManager* tm);
    virtual ~Render();

    void RenderSprite(Sprite* sp);
    void RenderTextSprite(TextSprite* sp, bool hd = false);
    void setCam(Camera cam);
    Camera* getCam();
    void resizeW(int w, int h);

    glm::mat4 projMat;
    float Zoom = 1.0f;

private:
    TextureManager* texman;
    Camera camActive;
    unsigned int boxVAO, tVAO, tVBO;
};

#endif // RENDER_H
