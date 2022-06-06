#include "Game/UI/ControlWindow.h"

#include "Engine.h"
#include <imgui.h>

void ControlWindow::Draw()
{
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({390, 210});
    ImGui::Begin("Параметры", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    {
        ImGui::Text("Скорость:");
        ImGui::SameLine();
        ImGui::InputFloat("##1", &Velocity);
        ImGui::Text("Угол:");
        ImGui::SameLine();
        ImGui::InputFloat("##2", &Angle);
        ImGui::Text("Время:");
        ImGui::SameLine();
        ImGui::SliderFloat("##3", &Engine::Get().GetTimer().TimeScale, MinTimeScale, MaxTimeScale, "%.1f");
        ImGui::SameLine();
        ImGui::Text("(x%.1f)", Engine::Get().GetTimer().TimeScale);
        ImGui::Text("Следовать:");
        ImGui::SameLine();
        ImGui::Checkbox("##5", &IsTrackBall);

        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 170) / 2);
        if (ImGui::ImageButton((ImTextureID)Engine::Get().GetResourceManager().GetTexture("StartPosIcon"), ImVec2(45,45)))
            IsToStartRequest = true;
        ImGui::SameLine();
        if (ImGui::ImageButton((ImTextureID)Engine::Get().GetResourceManager().GetTexture("MiddlePosIcon"), ImVec2(45,45)))
            IsToMiddleRequest = true;
        ImGui::SameLine();
        if (ImGui::ImageButton((ImTextureID)Engine::Get().GetResourceManager().GetTexture("EndPosIcon"), ImVec2(45,45)))
            IsToEndRequest = true;
    }
    ImGui::End();
}
