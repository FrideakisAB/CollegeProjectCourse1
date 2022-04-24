#include <windows.h>
#define GLM_FORCE_SSE3
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Timet.h"
#include "TextureManager.h"
#include "Render.h"
#include "Sprite.h"
#include "TextSprite.h"
#include <cmath>
#include <string>
#include <cstdio>
#include <imgui.h>
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <filesystem>

namespace fs = std::filesystem;

#define PI 3.14159265

static float value = 1.0f;
static bool physAct = false;
static float Vo = 18;
static float angle = 30;
static float sa = sin(angle * PI / 180);
static float ca = cos(angle * PI / 180);
static float tp = (Vo / 4.9f) * sa;
static float xmax = Vo * ca * tp;
static float ymax = Vo * sa * 0.5f * tp - 4.9f * 0.25f * tp * tp;
static float t = 0.f;
static float tr = 0.f;
static float ny = 1.0f;
static float nx = 1.0f;
static float zoom = 1.0f;
static float sensS = 0.03f;
static float x, y;
static int spCam = 80;
static std::vector<float> points;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    	physAct = !physAct;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    zoom = zoom - yoffset * sensS;
}

static char vel[32];
static int velLen;
static char angleS[32];
static int angleSLen;
float ratio_one[] = {0.2f, 0.8f};
float ratio_two[] = {0.3f, 0.5f, 0.2f};

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR str, int nWinMode)
{
    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "GLEngine", nullptr, nullptr);
    if(window == nullptr)
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
        std::cout << "ERROR::GLEW: Could not init GLEW" << std::endl;
        glfwTerminate();
        return -2;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if(fs::exists("resources/Arial_AMU.ttf"))
        io.Fonts->AddFontFromFileTTF("resources/Arial_AMU.ttf", 18.5f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    Time* time = new Time();
    TextureManager* tm = new TextureManager();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    Camera cam(glm::vec3(-80.0f, 0.0f, 1.0f));
    Render curr(width, height, tm);
    curr.setCam(cam);

    ///Game objects
    Sprite sp1("sp1");
    sp1.setTexture("Ball");
    sp1.x = 102; sp1.y = 101; sp1.z = -2; sp1.sx = 50; sp1.sy = 50;

    Sprite sp2("sp2");
    sp2.setTexture("PBall");
    sp2.x = 102; sp2.y = 101; sp2.z = -3; sp2.sx = 5; sp2.sy = 5;

    Sprite arrS("napr");
    arrS.setTexture("ArrowStart");
    arrS.x = 127; arrS.y = 135; arrS.z = -1; arrS.sx = 150; arrS.sy = 150;

    Sprite arrB("napr");
    arrB.setTexture("ArrowYBody");
    arrB.x = 127; arrB.y = 90 + (ymax * 15 + 120) * ny / 2; arrB.z = -1; arrB.sx = 150; arrB.sy = (ymax * 15 + 100) * ny;
    Sprite arrBX("napr");
    arrBX.setTexture("ArrowXBody");
    arrBX.x = 92 + (xmax * 15 + 100) * nx / 2; arrBX.y = 135; arrBX.z = -1; arrBX.sx = (xmax * 15 + 100) * nx; arrBX.sy = 150;

    Sprite arrYE("napr");
    arrYE.setTexture("ArrowYEnd");
    arrYE.x = 127; arrYE.y = 90 + (ymax * 15 + 120) * ny + 75; arrYE.z = -1; arrYE.sx = 150; arrYE.sy = 150;
    Sprite arrXE("napr");
    arrXE.setTexture("ArrowXEnd");
    arrXE.x = 92 + (xmax * 15 + 100) * nx + 75; arrXE.y = 135; arrXE.z = -1; arrXE.sx = 150; arrXE.sy = 150;

    TextSprite txtX("textXaxis");
    txtX.x = 102; txtX.y = 34; txtX.z = -2; txtX.sx = 200; txtX.sy = 200;
    txtX.setText("17893s");

    TextSprite txtY("textYaxis");
    txtY.x = 0; txtY.y = 34; txtY.z = -2; txtY.sx = 200; txtY.sy = 200;
    txtY.setText("17893s");
    ///Game objects end

    int i = 0;
    float gt = 0;

    float velF = Vo, angleF = angle;
    while(!glfwWindowShouldClose(window))
    {
        time->startFrame();
        glfwPollEvents();

        if(physAct && t != tp)
            glfwSetWindowTitle(window, "Симуляция...");
        else if(t != tp)
            glfwSetWindowTitle(window, "Пауза");
        else
            glfwSetWindowTitle(window, "Cимуляция окончена");

        if(velF != Vo || angleF != angle)
        {
            if(velF >= 0)
                Vo = velF;
            else
            {
                Vo = 0;
                snprintf(vel, sizeof vel, "%.0f", Vo);
            }

            if(angleF <= 90.0f && angleF >= 0)
                angle = angleF;
            else if(angleF > 90.0f)
            {
                angle = 90.0f;
                snprintf(angleS, sizeof angleS, "%.0f", angle);
            }
            else
            {
                angle = 0.0f;
                snprintf(angleS, sizeof angleS, "%.0f", angle);
            }
            sa = sin(angle * PI / 180);
            ca = cos(angle * PI / 180);
            tp = (Vo / 4.9f) * sa;
            xmax = Vo * ca * tp;
            ymax = Vo * sa * 0.5f * tp - 4.9f * 0.25f * tp * tp;

            t = tr;
            points.erase(points.begin(), points.end());
        }

        if(i<15)
        {
            i++;
        }

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        curr.Zoom = zoom;
        curr.resizeW(width, height);

        glClearColor(0.6f, 1.f, 0.68f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        arrB.y = 90 + (ymax * 15 + 120) * ny / 2; arrB.sy = (ymax * 15 + 120) * ny;
        arrBX.x = 92 + (xmax * 15 + 100) * nx / 2; arrBX.sx = (xmax * 15 + 100) * nx;

        arrYE.y = 90 + (ymax * 15 + 120) * ny + 75;
        arrXE.x = 92 + (xmax * 15 + 100) * nx + 75;

        for(int i = 0; i < points.size(); i+= 2)
        {
            sp2.x = points[i]; sp2.y = points[i + 1];
            curr.RenderSprite(&sp2);
            if(value < 0.7f)
                i+= 2;
        }
        curr.RenderSprite(&sp1);
        curr.RenderSprite(&arrB);
        curr.RenderSprite(&arrBX);
        curr.RenderSprite(&arrS);
        curr.RenderSprite(&arrYE);
        curr.RenderSprite(&arrXE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        curr.RenderTextSprite(&txtX);
        curr.RenderTextSprite(&txtY, true);

        //Physic block
        if(i > 10)
            gt += time->getDeltaTime();

        if(t != tp)
        {
            if(t < tp) {
                if(physAct)
                    t += gt * value;
            }
            else
            {
                t = tp;
            }

            x = Vo * ca * t;
            y = Vo * sa * t - 4.905f * t * t;

            sp1.x = 102 + x * 15.0f;
            if(y < 0.0f)
                y = 0;
            sp1.y = 101 + y * 15.0f;
            if(sp1.y < 101)
                sp1.y = 101;

            txtX.x = sp1.x;
            txtY.y = sp1.y - 10;
            std::ostringstream ss;
            ss << std::floor(x * 100 + 0.5) / 100;
            std::string s(ss.str() + "m");
            txtX.setText(s);
            std::ostringstream yss;
            yss << std::floor(y * 100 + 0.5) / 100;
            std::string ys(yss.str() + "m");
            txtY.setText(ys);

            points.push_back(sp1.x);
            points.push_back(sp1.y);
        }

        gt = 0;
        //Physic block

        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            spCam = 200;
        }
        else
        {
            spCam = 80;
        }

        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            t = tr;
            points.erase(points.begin(), points.end());
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP == GLFW_PRESS))
            curr.getCam()->Position.y += spCam * time->getDeltaTime();

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN == GLFW_PRESS))
            curr.getCam()->Position.y -= spCam * time->getDeltaTime();

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT == GLFW_PRESS))
            curr.getCam()->Position.x -= spCam * time->getDeltaTime();

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT == GLFW_PRESS))
            curr.getCam()->Position.x += spCam * time->getDeltaTime();

        curr.getCam()->updateCameraVectors();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

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
            ImGui::SliderFloat("##3", &value, 0.2f, 3.0f, "%.1f");
            ImGui::SameLine();
            ImGui::Text("(x%.1f)", value);
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
        time->endFrame();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    time->~Time();
    glfwTerminate();
    return 0;
}
