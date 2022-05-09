#include "Game/Simulation.h"

#include "Engine.h"
#include <imgui.h>
#include <algorithm>
#include <GLFW/glfw3.h>
#include "Game/UI/InfoWindow.h"

Simulation::Simulation()
{
    controlWindow.Velocity = startSpeed;
    controlWindow.Angle = angle;
}

void Simulation::OnUpdate()
{
    SetTitleFromState();
    SimulationIterate();
    CameraControl();
    ParseInput();
}

void Simulation::OnUIRender()
{
    InfoWindow::Draw();
    controlWindow.Draw();
    RenderOnUI();
}

void Simulation::OnScroll(double yOffset)
{
    Engine::Get().GetRender().Zoom = std::clamp(Engine::Get().GetRender().Zoom - static_cast<float>(yOffset) * zoomSens, zoomMin, zoomMax);
}

void Simulation::OnKeyInput(int key, int action)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(Engine::Get().GetWindowPtr(), GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        isSimulationActive = !isSimulationActive;
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        simulationTime = 0.f;
        points.erase(points.begin(), points.end());
    }
}

void Simulation::CameraControl()
{
    float dt = Engine::Get().GetTimer().GetUnscaleDeltaTime();
    float zoom = Engine::Get().GetRender().Zoom;

    if (glfwGetKey(Engine::Get().GetWindowPtr(), GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(Engine::Get().GetWindowPtr(), GLFW_KEY_UP == GLFW_PRESS))
    {
        float deltaPosition = zoom * cameraSpeed * dt;
        if (glfwGetKey(Engine::Get().GetWindowPtr(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            deltaPosition = deltaPosition * cameraShiftSpeed;
        Camera::Main.value().get().Position.y += deltaPosition;
    }

    if (glfwGetKey(Engine::Get().GetWindowPtr(), GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(Engine::Get().GetWindowPtr(), GLFW_KEY_DOWN == GLFW_PRESS))
    {
        float deltaPosition = zoom * cameraSpeed * dt;
        if (glfwGetKey(Engine::Get().GetWindowPtr(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            deltaPosition = deltaPosition * cameraShiftSpeed;
        Camera::Main.value().get().Position.y -= deltaPosition;
    }

    if (glfwGetKey(Engine::Get().GetWindowPtr(), GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(Engine::Get().GetWindowPtr(), GLFW_KEY_LEFT == GLFW_PRESS))
    {
        float deltaPosition = zoom * cameraSpeed * dt;
        if (glfwGetKey(Engine::Get().GetWindowPtr(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            deltaPosition = deltaPosition * cameraShiftSpeed;
        Camera::Main.value().get().Position.x -= deltaPosition;
    }

    if (glfwGetKey(Engine::Get().GetWindowPtr(), GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(Engine::Get().GetWindowPtr(), GLFW_KEY_RIGHT == GLFW_PRESS))
    {
        float deltaPosition = zoom * cameraSpeed * dt;
        if (glfwGetKey(Engine::Get().GetWindowPtr(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            deltaPosition = deltaPosition * cameraShiftSpeed;
        Camera::Main.value().get().Position.x += deltaPosition;
    }
}

void Simulation::PostInit()
{
    camera = std::make_unique<Camera>(glm::vec3(-80.0f, 0.0f, 1.0f));

    ball = std::make_unique<Sprite>("Ball");
    ball->Texture ="Ball";
    ball->Position = glm::vec3(102, 101, -2);
    ball->Scale = glm::vec2(50, 50);

    falseBall = std::make_unique<Sprite>("FalseBall");
    falseBall->Texture ="PBall";
    falseBall->Position = glm::vec3(102, 101, -3);
    falseBall->Scale = glm::vec2(5, 5);

    arrowStart = std::make_unique<Sprite>("ArrowStart");
    arrowStart->Texture ="ArrowStart";
    arrowStart->Position = glm::vec3(127, 135, -1);
    arrowStart->Scale = glm::vec2(150, 150);

    arrowYBody = std::make_unique<Sprite>("ArrowYBody");
    arrowYBody->Texture ="ArrowYBody";
    arrowYBody->Position = glm::vec3(127, 90 + (maxHeight * 15 + 120) * yScale / 2, -1);
    arrowYBody->Scale = glm::vec2(150, (maxHeight * 15 + 120) * yScale);
    arrowXBody = std::make_unique<Sprite>("ArrowXBody");
    arrowXBody->Texture ="ArrowXBody";
    arrowXBody->Position = glm::vec3(92 + (throwDistance * 15 + 100) * xScale / 2, 135, -1);
    arrowXBody->Scale = glm::vec2((throwDistance * 15 + 100) * xScale, 150);

    arrowYEnd = std::make_unique<Sprite>("ArrowYEnd");
    arrowYEnd->Texture ="ArrowYEnd";
    arrowYEnd->Position = glm::vec3(127, 90 + (maxHeight * 15 + 120) * yScale + 75, -1);
    arrowYEnd->Scale = glm::vec2(150, 150);
    arrowXEnd = std::make_unique<Sprite>("ArrowXEnd");
    arrowXEnd->Texture ="ArrowXEnd";
    arrowXEnd->Position = glm::vec3(92 + (throwDistance * 15 + 100) * xScale + 75, 135, -1);
    arrowXEnd->Scale = glm::vec2(150, 150);

    textX = std::make_unique<TextSprite>("textXAxis");
    textX->Position = glm::vec3(102, 34, -2);
    textX->Scale = glm::vec2(200, 200);
    textX->Text = "17893s";

    textY = std::make_unique<TextSprite>("textYAxis");
    textY->Position = glm::vec3(0, 34, -2);
    textY->Scale = glm::vec2(200, 200);
    textY->Text = "17893s";
}

void Simulation::ParseInput()
{
    if (controlWindow.Velocity != startSpeed || controlWindow.Angle != angle)
    {
        if (controlWindow.Velocity >= 0)
            startSpeed = controlWindow.Velocity;
        else
        {
            startSpeed = 0;
            controlWindow.Velocity = 0;
        }

        if (controlWindow.Angle <= 90.0f && controlWindow.Angle >= 0)
            angle = controlWindow.Angle;
        else if (controlWindow.Angle > 90.0f)
        {
            angle = 90.0f;
            controlWindow.Angle = 90.0f;
        }
        else
        {
            angle = 0.0f;
            controlWindow.Angle = 0.0f;
        }

        CalculateFromValues();
    }
}

void Simulation::CalculateFromValues()
{
    sinAngle = sin(angle * PI / 180);
    cosAngle = cos(angle * PI / 180);
    simulationEndTime = (startSpeed / halfG) * sinAngle;
    throwDistance = startSpeed * cosAngle * simulationEndTime;
    maxHeight = startSpeed * sinAngle * 0.5f * simulationEndTime - halfG * 0.25f * simulationEndTime * simulationEndTime;

    simulationTime = 0.f;
    points.erase(points.begin(), points.end());

    arrowYBody->Position.y = 90 + (maxHeight * 15 + 120) * yScale / 2;
    arrowYBody->Scale.y = (maxHeight * 15 + 120) * yScale;

    arrowXBody->Position.x = 92 + (throwDistance * 15 + 100) * xScale / 2;
    arrowXBody->Scale.x = (throwDistance * 15 + 100) * xScale;

    arrowYEnd->Position.y = 90 + (maxHeight * 15 + 120) * yScale + 75;
    arrowXEnd->Position.x = 92 + (throwDistance * 15 + 100) * xScale + 75;
}

void Simulation::OnRender()
{
    Render &render = Engine::Get().GetRender();

    render.RenderSprite(*ball);
    render.RenderSprite(*arrowYBody);
    render.RenderSprite(*arrowXBody);
    render.RenderSprite(*arrowStart);
    render.RenderSprite(*arrowYEnd);
    render.RenderSprite(*arrowXEnd);

    for (auto &point : points)
    {
        falseBall->Position.x = point.x;
        falseBall->Position.y = point.y;
        render.RenderSprite(*falseBall);
    }
}

void Simulation::RenderOnUI() const
{
    Render &render = Engine::Get().GetRender();

    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({static_cast<float>(render.GetWindowSize().x), static_cast<float>(render.GetWindowSize().y)});
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("##1", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
    render.RenderTextSprite(*textX);
    render.RenderTextSprite(*textY);
    ImGui::End();
}

void Simulation::SetTitleFromState() const
{
    if (isSimulationActive && simulationTime != simulationEndTime)
        glfwSetWindowTitle(Engine::Get().GetWindowPtr(), "Симуляция...");
    else if (simulationTime != simulationEndTime)
        glfwSetWindowTitle(Engine::Get().GetWindowPtr(), "Пауза");
    else
        glfwSetWindowTitle(Engine::Get().GetWindowPtr(), "Cимуляция окончена");
}

void Simulation::SimulationIterate()
{
    if (simulationTime != simulationEndTime)
    {
        if (simulationTime < simulationEndTime)
        {
            if (isSimulationActive)
                simulationTime += Engine::Get().GetTimer().GetDeltaTime();
        }
        else
            simulationTime = simulationEndTime;

        x = startSpeed * cosAngle * simulationTime;
        y = startSpeed * sinAngle * simulationTime - halfG * simulationTime * simulationTime;

        ball->Position.x = 102 + x * 15.0f;
        if (y < 0.0f)
            y = 0;
        ball->Position.y = 101 + y * 15.0f;
        if (ball->Position.y < 101)
            ball->Position.y = 101;

        textX->Position.x = ball->Position.x;
        textY->Position.y = ball->Position.y - 10;

        std::stringstream ss;
        ss << "x = " << std::floor(x * 100 + 0.5) / 100 << 'm';
        textX->Text = ss.str();

        ss = std::stringstream();

        ss << "y = " << std::floor(y * 100 + 0.5) / 100 << 'm';
        textY->Text = ss.str();

        points.emplace_back(ball->Position.x, ball->Position.y);
    }
}
