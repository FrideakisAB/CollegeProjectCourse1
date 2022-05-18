#include "Render/TextSpriteTask.h"

#include <imgui.h>
#include "Engine.h"
#include "Game/Camera.h"
#include "Game/TextSprite.h"

TextSpriteTask::TextSpriteTask(const TextSprite &sprite)
{
    color = sprite.Color;
    text = sprite.Text;

    Position = sprite.Position;
    Scale = sprite.Scale;
}

void TextSpriteTask::UIDraw()
{
    glm::vec4 position = Engine::Get().GetRender().ProjectMat * Camera::Main.value().get().GetViewMatrix() * glm::vec4(Position.x, Position.y, Position.z, 1.0f);

    auto size = ImGui::GetWindowSize();

    auto textSize = ImGui::CalcTextSize(text.data());

    glm::vec2 screenPosition = glm::vec2(position.x * size.x / 2 + size.x / 2 - textSize.x / 2, size.y - (position.y * size.y / 2 + size.y / 2) - textSize.y / 2);

    screenPosition.x = glm::clamp(screenPosition.x, textSize.x / 2, size.x - textSize.x);
    screenPosition.y = glm::clamp(screenPosition.y, textSize.y / 2, size.y - textSize.y);

    ImGui::SetCursorPos({screenPosition.x , screenPosition.y});
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.r, color.g, color.b, 1.0f));
    ImGui::Text(text.data());
    ImGui::PopStyleColor();
}
