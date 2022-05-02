#include "Game/UI/ControlWindow.h"

#include "Engine.h"
#include <imgui.h>

void ControlWindow::Draw()
{
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({365, 130});
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
    }
    ImGui::End();
}
