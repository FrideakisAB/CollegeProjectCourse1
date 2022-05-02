#include "Engine.h"

Engine::Engine()
{
    if (IsValid())
        throw new std::exception("Dont create duplicate Engine");

    instance = this;
}

Engine::~Engine()
{
    if (instance == this)
        instance = nullptr;
}

void Engine::OnUpdate()
{

}

void Engine::OnRender()
{

}

void Engine::OnUIRender()
{

}
