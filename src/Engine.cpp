#include "Engine.h"

#include "Log.h"
#include <imgui.h>

Engine::Engine(GLFWwindow *window) :
    window(window)
{
    if (IsValid())
    {
        Log::Get().Error("Trying to Duplicate Engine, not allowed");
        throw std::exception("Dont create duplicate Engine");
    }

    instance = this;

    simulation.PostInit();
}

Engine::~Engine()
{
    if (instance == this)
        instance = nullptr;
}

void Engine::OnUpdate()
{
    simulation.OnUpdate();
}

void Engine::OnRender()
{
    simulation.OnRender();
    render.Draw();
}

void Engine::OnUIRender()
{
    simulation.OnUIRender();

    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({static_cast<float>(render.GetWindowSize().x), static_cast<float>(render.GetWindowSize().y)});
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("##1", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
    render.UIDraw();
    ImGui::End();
}

void Engine::OnScroll(double yOffset)
{
    simulation.OnScroll(yOffset);
}

void Engine::OnKeyInput(int key, int action)
{
    simulation.OnKeyInput(key, action);
}
