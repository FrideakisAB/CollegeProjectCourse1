#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Timer.h"
#include "ResourceManager.h"
#include "Render/Render.h"
#include <imgui.h>
#include <filesystem>
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Engine.h"

namespace fs = std::filesystem;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

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

    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

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
    if (fs::exists("resources/Arial_AMU.ttf"))
        io.Fonts->AddFontFromFileTTF("resources/Arial_AMU.ttf", 18.5f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Engine engine(window);

    Timer &timer = engine.GetTimer();
    ResourceManager &resourceManager = engine.GetResourceManager();

    int width, height;

    Render &render = engine.GetRender();
    while (!glfwWindowShouldClose(window))
    {
        timer.Tick();
        glfwPollEvents();

        engine.OnUpdate();

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        render.ResizeWindow(width, height);

        glClearColor(0.6f, 1.f, 0.68f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        engine.OnRender();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        engine.OnUIRender();

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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    Engine::Get().OnKeyInput(key, action);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    Engine::Get().OnScroll(yOffset);
}
