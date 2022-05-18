#include "Render/Render.h"

#include <imgui.h>
#include "Engine.h"

Render::Render(int width, int height) :
    width(width), height(height)
{
    ProjectMat = glm::ortho(0.f, width * Zoom + 0.f, 0.f, height * Zoom + 0.f, 0.1f, 100.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void Render::ResizeWindow(int width, int height)
{
    this->width = width;
    this->height = height;
    ProjectMat = glm::ortho(0.f, width * Zoom + 0.f, 0.f, height * Zoom + 0.f, 0.1f, 100.0f);
}

void Render::Draw()
{
    for (const auto &[shaderName, taskVector] : renderTasks)
    {
        for (IRenderTask *task : taskVector)
        {
            const Shader &shader = Engine::Get().GetResourceManager().GetShader(shaderName.data());

            shader.Use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, task->Position);
            model = glm::scale(model, glm::vec3(task->Scale.x, task->Scale.y, 1));

            shader.SetMat4("mvp", ProjectMat * Camera::Main.value().get().GetViewMatrix() * model);

            task->Draw(shader);
        }
    }

    renderTasks.clear();
}

void Render::UIDraw()
{
    for (IRenderTask *task : uiTasks)
        task->UIDraw();
}

void Render::AddRenderTask(IRenderTask *task)
{
    if (task != nullptr)
        renderTasks[task->ShaderName].emplace_back(task);
}

void Render::AddUITask(IRenderTask *task)
{
    if (task != nullptr)
        uiTasks.emplace_back(task);
}
