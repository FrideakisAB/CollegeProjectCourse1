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

void Render::RenderTextSprite(const TextSprite &sprite) const
{
    glm::vec4 position = ProjectMat * Camera::Main.value().get().GetViewMatrix() * glm::vec4(sprite.Position.x, sprite.Position.y, sprite.Position.z, 1.0f);

    auto size = ImGui::GetWindowSize();

    auto textSize = ImGui::CalcTextSize(sprite.Text.c_str());

    glm::vec2 screenPosition = glm::vec2(position.x * size.x / 2 + size.x / 2 - textSize.x / 2, size.y - (position.y * size.y / 2 + size.y / 2) - textSize.y / 2);

    screenPosition.x = glm::clamp(screenPosition.x, textSize.x / 2, size.x - textSize.x);
    screenPosition.y = glm::clamp(screenPosition.y, textSize.y / 2, size.y - textSize.y);

    ImGui::SetCursorPos({screenPosition.x , screenPosition.y});
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(sprite.Color.r, sprite.Color.g, sprite.Color.b, 1.0f));
    ImGui::Text(sprite.Text.c_str());
    ImGui::PopStyleColor();
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
    //TODO: make on add UITask
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
