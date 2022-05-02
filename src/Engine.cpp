#include "Engine.h"

Engine::Engine(GLFWwindow *window) :
    window(window)
{
    if (IsValid())
        throw new std::exception("Dont create duplicate Engine");

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
