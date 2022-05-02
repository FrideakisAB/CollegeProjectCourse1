#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Timer.h"
#include "ResourceManager.h"
#include "Render/Render.h"
#include "Game/Sprite.h"
#include "Game/TextSprite.h"
#include <cmath>
#include <string>
#include <imgui.h>
#include <filesystem>
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Engine.h"

namespace fs = std::filesystem;

#define PI 3.14159265

bool isSimulationActive = false;
const float yScale = 1.f;
const float xScale = 1.f;

float zoom = 1.0f;
const float zoomSens = 0.03f;
const float zoomMin = 0.2f;
const float zoomMax = 3.f;

const float cameraSpeed = 80.f;
const float cameraShiftSpeed = 2.5f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR str, int nWinMode)
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "GLEngine", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return -2;
    }
    glfwMakeContextCurrent(window);

    glfwSetWindowSizeLimits(window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSwapInterval(0);

    GLFWimage winIcon;
    int comp;
    winIcon.pixels = stbi_load("resources/textures/winIcon.png", &winIcon.width, &winIcon.height, &comp, 4);
    glfwSetWindowIcon(window, 1, &winIcon);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -3;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if(fs::exists("resources/Arial_AMU.ttf"))
        io.Fonts->AddFontFromFileTTF("resources/Arial_AMU.ttf", 18.5f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Engine engine;

    Timer &timer = engine.GetTimer();
    ResourceManager &resourceManager = engine.GetResourceManager();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    Camera cam(glm::vec3(-80.0f, 0.0f, 1.0f));
    Render curr(width, height);

    //Simulation block
    float startSpeed = 18;
    static float angle = 30;
    static float sinAngle = sin(angle * PI / 180);
    static float cosAngle = cos(angle * PI / 180);

    float simulationTime = 0.f;
    float simulationEndTime = (startSpeed / 4.9f) * sinAngle;

    float throwDistance = startSpeed * cosAngle * simulationEndTime;
    float maxHeight = startSpeed * sinAngle * 0.5f * simulationEndTime - 4.9f * 0.25f * simulationEndTime * simulationEndTime;

    float x, y;

    std::vector<float> points;
    //

    ///Game objects
    Sprite sp1("sp1");
    sp1.Texture ="Ball";
    sp1.Position = glm::vec3(102, 101, -2);
    sp1.Scale = glm::vec2(50, 50);

    Sprite sp2("sp2");
    sp2.Texture ="PBall";
    sp2.Position = glm::vec3(102, 101, -3);
    sp2.Scale = glm::vec2(5, 5);

    Sprite arrS("napr");
    arrS.Texture ="ArrowStart";
    arrS.Position = glm::vec3(127, 135, -1);
    arrS.Scale = glm::vec2(150, 150);

    Sprite arrB("napr");
    arrB.Texture ="ArrowYBody";
    arrB.Position = glm::vec3(127, 90 + (maxHeight * 15 + 120) * yScale / 2, -1);
    arrB.Scale = glm::vec2(150, (maxHeight * 15 + 100) * yScale);
    Sprite arrBX("napr");
    arrBX.Texture ="ArrowXBody";
    arrBX.Position = glm::vec3(92 + (throwDistance * 15 + 100) * xScale / 2, 135, -1);
    arrBX.Scale = glm::vec2((throwDistance * 15 + 100) * xScale, 150);

    Sprite arrYE("napr");
    arrYE.Texture ="ArrowYEnd";
    arrYE.Position = glm::vec3(127, 90 + (maxHeight * 15 + 120) * yScale + 75, -1);
    arrYE.Scale = glm::vec2(150, 150);
    Sprite arrXE("napr");
    arrXE.Texture ="ArrowXEnd";
    arrXE.Position = glm::vec3(92 + (throwDistance * 15 + 100) * xScale + 75, 135, -1);
    arrXE.Scale = glm::vec2(150, 150);

    TextSprite txtX("textXaxis");
    txtX.Position = glm::vec3(102, 34, -2);
    txtX.Scale = glm::vec2(200, 200);
    txtX.Text = "17893s";

    TextSprite txtY("textYaxis");
    txtX.Position = glm::vec3(0, 34, -2);
    txtX.Scale = glm::vec2(200, 200);
    txtX.Text = "17893s";
    ///Game objects end

    float velF = startSpeed, angleF = angle;
    while (!glfwWindowShouldClose(window))
    {
        timer.Tick();
        glfwPollEvents();

        engine.OnUpdate();

        if (isSimulationActive && simulationTime != simulationEndTime)
            glfwSetWindowTitle(window, "Симуляция...");
        else if (simulationTime != simulationEndTime)
            glfwSetWindowTitle(window, "Пауза");
        else
            glfwSetWindowTitle(window, "Cимуляция окончена");

        if (velF != startSpeed || angleF != angle)
        {
            if (velF >= 0)
                startSpeed = velF;
            else
            {
                startSpeed = 0;
                velF = 0;
            }

            if (angleF <= 90.0f && angleF >= 0)
                angle = angleF;
            else if (angleF > 90.0f)
            {
                angle = 90.0f;
                angleF = 90.0f;
            }
            else
            {
                angle = 0.0f;
                angleF = 0.0f;
            }

            sinAngle = sin(angle * PI / 180);
            cosAngle = cos(angle * PI / 180);
            simulationEndTime = (startSpeed / 4.9f) * sinAngle;
            throwDistance = startSpeed * cosAngle * simulationEndTime;
            maxHeight = startSpeed * sinAngle * 0.5f * simulationEndTime - 4.9f * 0.25f * simulationEndTime * simulationEndTime;

            simulationTime = 0.f;
            points.erase(points.begin(), points.end());
        }

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        curr.Zoom = zoom;
        curr.ResizeWindow(width, height);

        glClearColor(0.6f, 1.f, 0.68f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        arrB.Position.y = 90 + (maxHeight * 15 + 120) * yScale / 2; arrB.Scale.y = (maxHeight * 15 + 120) * yScale;
        arrBX.Position.x = 92 + (throwDistance * 15 + 100) * xScale / 2; arrBX.Scale.x = (throwDistance * 15 + 100) * xScale;

        arrYE.Position.y = 90 + (maxHeight * 15 + 120) * yScale + 75;
        arrXE.Position.x = 92 + (throwDistance * 15 + 100) * xScale + 75;

        for (int i = 0; i < points.size(); i+= 2)
        {
            sp2.Position.x = points[i]; sp2.Position.y = points[i + 1];
            curr.RenderSprite(&sp2);
        }

        engine.OnRender();

        curr.RenderSprite(&sp1);
        curr.RenderSprite(&arrB);
        curr.RenderSprite(&arrBX);
        curr.RenderSprite(&arrS);
        curr.RenderSprite(&arrYE);
        curr.RenderSprite(&arrXE);

        //Physic block
        if (simulationTime != simulationEndTime)
        {
            if (simulationTime < simulationEndTime)
            {
                if (isSimulationActive)
                    simulationTime += timer.GetDeltaTime();
            }
            else
                simulationTime = simulationEndTime;

            x = startSpeed * cosAngle * simulationTime;
            y = startSpeed * sinAngle * simulationTime - 4.905f * simulationTime * simulationTime;

            sp1.Position.x = 102 + x * 15.0f;
            if (y < 0.0f)
                y = 0;
            sp1.Position.y = 101 + y * 15.0f;
            if (sp1.Position.y < 101)
                sp1.Position.y = 101;

            txtX.Position.x = sp1.Position.x;
            txtY.Position.y = sp1.Position.y - 10;

            std::stringstream ss;
            ss << std::floor(x * 100 + 0.5) / 100 << 'm';
            txtX.Text = ss.str();

            ss = std::stringstream();

            ss << std::floor(y * 100 + 0.5) / 100 << 'm';
            txtY.Text = ss.str();

            points.push_back(sp1.Position.x);
            points.push_back(sp1.Position.y);
        }
        //Physic block

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            simulationTime = 0.f;
            points.erase(points.begin(), points.end());
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP == GLFW_PRESS))
        {
            float deltaPosition = zoom * cameraSpeed * timer.GetUnscaleDeltaTime();
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                deltaPosition = deltaPosition * cameraShiftSpeed;
            Camera::Main.value().get().Position.y += deltaPosition;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN == GLFW_PRESS))
        {
            float deltaPosition = zoom * cameraSpeed * timer.GetUnscaleDeltaTime();
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                deltaPosition = deltaPosition * cameraShiftSpeed;
            Camera::Main.value().get().Position.y -= deltaPosition;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT == GLFW_PRESS))
        {
            float deltaPosition = zoom * cameraSpeed * timer.GetUnscaleDeltaTime();
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                deltaPosition = deltaPosition * cameraShiftSpeed;
            Camera::Main.value().get().Position.x -= deltaPosition;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT == GLFW_PRESS))
        {
            float deltaPosition = zoom * cameraSpeed * timer.GetUnscaleDeltaTime();
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                deltaPosition = deltaPosition * cameraShiftSpeed;
            Camera::Main.value().get().Position.x += deltaPosition;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        engine.OnUIRender();

        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize({static_cast<float>(width), static_cast<float>(height)});
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::Begin("##1", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                                     ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
        curr.RenderTextSprite(&txtX);
        curr.RenderTextSprite(&txtY);
        ImGui::End();

        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize({365, 130});
        ImGui::Begin("Параметры", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        {
            ImGui::Text("Скорость:");
            ImGui::SameLine();
            ImGui::InputFloat("##1", &velF);
            ImGui::Text("Угол:");
            ImGui::SameLine();
            ImGui::InputFloat("##2", &angleF);
            ImGui::Text("Время:");
            ImGui::SameLine();
            ImGui::SliderFloat("##3", &timer.TimeScale, 0.2f, 3.0f, "%.1f");
            ImGui::SameLine();
            ImGui::Text("(x%.1f)", timer.TimeScale);
        }
        ImGui::End();

        ImGui::SetNextWindowPos({static_cast<float>(width - 430), 0});
        ImGui::SetNextWindowSize({430, 250});
        ImGui::Begin("Управление", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        {
            ImGui::Text("Процесс");
            if (ImGui::BeginTable("Процесс", 2))
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("Esc").x) / 2);
                ImGui::TextColored({200, 30, 0, 255}, "Esc");
                ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("R").x) / 2);
                ImGui::TextColored({200, 30, 0, 255}, "R");
                ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("Пробел").x) / 2);
                ImGui::TextColored({200, 30, 0, 255}, "Пробел");

                ImGui::TableNextColumn();

                ImGui::TextColored({255, 255, 206, 255}, "выход");
                ImGui::TextColored({255, 255, 206, 255}, "повтор запуска");
                ImGui::TextColored({255, 255, 206, 255}, "старт/пауза");

                ImGui::EndTable();
            }

            ImGui::Text("Перемещение камеры");
            if (ImGui::BeginTable("Перемещение камеры", 2))
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("W,A,S,D").x) / 2);
                ImGui::TextColored({200, 30, 0, 255}, "W,A,S,D");
                ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("W,A,S,D").x) / 2);
                ImGui::TextColored({200, 30, 0, 255}, "Стрелки");
                ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("Shift").x) / 2);
                ImGui::TextColored({200, 30, 0, 255}, "Shift");
                ImGui::SetCursorPosX((ImGui::GetColumnWidth() - ImGui::CalcTextSize("Колёсико").x) / 2);
                ImGui::TextColored({200, 30, 0, 255}, "Колёсико");

                ImGui::TableNextColumn();

                ImGui::TextColored({255, 255, 206, 255}, "вверх, влево, вниз, вправо");
                ImGui::TextColored({255, 255, 206, 255}, "вверх, влево, вниз, вправо");
                ImGui::TextColored({255, 255, 206, 255}, "ускорить движение камеры");
                ImGui::TextColored({255, 255, 206, 255}, "приблизить/отдалить");

                ImGui::EndTable();
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        isSimulationActive = !isSimulationActive;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    zoom = std::clamp(zoom - static_cast<float>(yoffset) * zoomSens, zoomMin, zoomMax);
}
