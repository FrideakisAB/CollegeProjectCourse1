#ifndef RENDER_H
#define RENDER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"
#include "Game/Sprite.h"
#include "Game/TextSprite.h"
#include "Game/Camera.h"
#include "Game/LinesSprite.h"
#include "Render/IRenderTask.h"

class Render {
public:
    Render(int width, int height);

    void ResizeWindow(int width, int height);

    void Draw();
    void UIDraw();

    void AddRenderTask(IRenderTask *task);
    void AddUITask(IRenderTask *task);

    glm::mat4 ProjectMat = glm::mat4(1.f);
    float Zoom = 1.0f;

    [[nodiscard]] glm::ivec2 GetWindowSize() const noexcept { return {width, height}; }

private:
    int width, height;

    std::map<std::string_view, std::vector<IRenderTask*>> renderTasks;
    std::vector<IRenderTask*> uiTasks;
};

#endif
