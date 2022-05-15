#include "Engine.h"

#include "Log.h"

Engine::Engine(GLFWwindow *window) :
    window(window)
{
    if (IsValid())
    {
        Log::Get().Error("Trying to Duplicate Engine, not allowed");
        throw new std::exception("Dont create duplicate Engine");
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
}

void Engine::OnUIRender()
{
    simulation.OnUIRender();
}

void Engine::OnScroll(double yOffset)
{
    simulation.OnScroll(yOffset);
}

void Engine::OnKeyInput(int key, int action)
{
    simulation.OnKeyInput(key, action);
}
