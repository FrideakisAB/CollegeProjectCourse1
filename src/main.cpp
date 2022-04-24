#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Timet.h"
#include "TextureManager.h"
#include "Render.h"
#include "Sprite.h"
#include "TextSprite.h"
#include <sys/types.h>
#include <dirent.h>
#include <cmath>
#include <string>
#include <wchar.h>
#include <stdio.h>
#include <math.h>
#include <glm/gtc/type_ptr.hpp>

#define NKCD NKC_GLFW
#define NKC_USE_OPENGL 3
#define NKC_IMPLEMENTATION
#include "nuklear_cross/nuklear_cross.h"
#include "nuklear_cross/nkc_frontend/nkc_glfw.h"
#define NK_GLFW_GL3_IMPLEMENTATION
#include "nuklear_cross/nuklear_drivers/nuklear_glfw_gl3.h"


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

int main() {
    nkc nkcHandle;
    nk_context *ctx = nkc_init(&nkcHandle, "Simulation...", 1024, 720, NKC_WIN_NORMAL);
    glfwSetKeyCallback(nkcHandle.window, key_callback);
    glfwSetScrollCallback(nkcHandle.window, scroll_callback);

    GLFWimage winIcon;
    int comp;
    winIcon.pixels = stbi_load("resources/textures/winIcon.png", &winIcon.width, &winIcon.height, &comp, 4);
    glfwSetWindowIcon(nkcHandle.window, 1, &winIcon);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR::GLEW: Could not init GLEW" << std::endl;
        nkc_shutdown(&nkcHandle);
        return -2;
    }

    Time* time = new Time();
    TextureManager* tm = new TextureManager();

    int width, height;
    glfwGetFramebufferSize(nkcHandle.window, &width, &height);
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

    if(!nkc_load_font_file(&nkcHandle, "resources/Arial_AMU.ttf", 18, nk_font_cyrillic_glyph_ranges()))
        std::cout << "ERROR::FONT" << std::endl;

    snprintf(vel, sizeof vel, "%.0f", Vo);
    snprintf(angleS, sizeof angleS, "%.0f", angle);

    while(!glfwWindowShouldClose(nkcHandle.window))
    {
        time->startFrame();
        nkc_poll_events(&nkcHandle);

        if(physAct && t != tp)
            glfwSetWindowTitle(nkcHandle.window, u8"Симуляция...");
        else if(t != tp)
            glfwSetWindowTitle(nkcHandle.window, u8"Пауза");
        else
            glfwSetWindowTitle(nkcHandle.window, u8"Cимуляция окончена");

        zoom = zoom - glfw.scroll.y * sensS;

        std::string vs(vel);
        float velF;
        if(vs != "")
            velF = std::stof(vs);
        std::string as(angleS);
        float angleF;
        if(as != "")
            angleF = std::stof(as);

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

        glfwGetFramebufferSize(nkcHandle.window, &width, &height);
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

        if(glfwGetKey(nkcHandle.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            spCam = 200;
        }
        else
        {
            spCam = 80;
        }

        if(glfwGetKey(nkcHandle.window, GLFW_KEY_R) == GLFW_PRESS)
        {
            t = tr;
            points.erase(points.begin(), points.end());
        }

        if (glfwGetKey(nkcHandle.window, GLFW_KEY_W) == GLFW_PRESS)
            curr.getCam()->Position.y += spCam * time->getDeltaTime();

        if (glfwGetKey(nkcHandle.window, GLFW_KEY_S) == GLFW_PRESS)
            curr.getCam()->Position.y -= spCam * time->getDeltaTime();

        if (glfwGetKey(nkcHandle.window, GLFW_KEY_A) == GLFW_PRESS)
            curr.getCam()->Position.x -= spCam * time->getDeltaTime();

        if (glfwGetKey(nkcHandle.window, GLFW_KEY_D) == GLFW_PRESS)
            curr.getCam()->Position.x += spCam * time->getDeltaTime();

        std::stringstream ss;
        ss << round((sp1.x - 102) * 10) / 10;

        //std::cout << 1 / time->getDeltaTime() << std::endl;

        curr.getCam()->updateCameraVectors();
                                                           //120
        if (nk_begin(ctx, "ControlBlock", nk_rect(0, 0, 365, 150), NK_WINDOW_BORDER))
        {
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, u8"Параметры", NK_TEXT_ALIGN_CENTERED);

            nk_layout_row(ctx, NK_DYNAMIC, 30, 2, ratio_one);
            nk_label(ctx, u8"Скорость:", NK_TEXT_RIGHT);
            nk_edit_string_zero_terminated(ctx, NK_EDIT_SIMPLE, vel,/* &velLen,*/ sizeof(vel), nk_filter_float);

            nk_layout_row(ctx, NK_DYNAMIC, 30, 2, ratio_one);
            nk_label(ctx, u8"Угол:", NK_TEXT_RIGHT);
            nk_edit_string_zero_terminated(ctx, NK_EDIT_SIMPLE, angleS,/* &angleSLen,*/ sizeof(angleS), nk_filter_float);

            nk_layout_row_begin(ctx, NK_STATIC, 30, 3);
            {
                nk_layout_row_push(ctx, 50);
                nk_label(ctx, u8"Время:", NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 230);
                nk_slider_float(ctx, 0.2f, &value, 3.0f, 0.2f);
                std::string str;
                str += "(x";
                char array[3];
                sprintf(array, "%.1f", value);
                str += array;
                str += ")";
                nk_layout_row_push(ctx, 40);
                nk_label(ctx, str.c_str(), NK_TEXT_RIGHT);
            }
            nk_layout_row_end(ctx);

            //char array[16];
            //sprintf(array, "FPS:%.0f", floor(1 / time->getDeltaTime()));

            //nk_layout_row_dynamic(ctx, 25, 1);
            //nk_label(ctx, array, NK_TEXT_ALIGN_CENTERED);

        }
        nk_end(ctx);

        if (nk_begin(ctx, "InfoBlock", nk_rect(width - 300, 0, 300, 250),
            NK_WINDOW_BORDER/*|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE*/))
        {
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, u8"Клавиши", NK_TEXT_ALIGN_CENTERED);

            nk_layout_row(ctx, NK_DYNAMIC, 25, 2, ratio_one);
            nk_label_colored(ctx, u8"Esc", NK_TEXT_ALIGN_CENTERED, nk_rgb(200, 30, 0));
            nk_label_colored(ctx, u8"выход", NK_TEXT_ALIGN_CENTERED, nk_rgb(255, 255, 206));

            nk_layout_row(ctx, NK_DYNAMIC, 25, 2, ratio_one);
            nk_label_colored(ctx, u8"R", NK_TEXT_ALIGN_CENTERED, nk_rgb(200, 30, 0));
            nk_label_colored(ctx, u8"повтор запуска", NK_TEXT_ALIGN_CENTERED, nk_rgb(255, 255, 206));

            nk_layout_row(ctx, NK_DYNAMIC, 25, 2, ratio_one);
            nk_label_colored(ctx, u8"Пробел", NK_TEXT_ALIGN_CENTERED, nk_rgb(200, 30, 0));
            nk_label_colored(ctx, u8"старт/пауза", NK_TEXT_ALIGN_CENTERED, nk_rgb(255, 255, 206));

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, u8"  Перемещение камеры:", NK_TEXT_ALIGN_LEFT);

            nk_layout_row(ctx, NK_DYNAMIC, 25, 2, ratio_one);
            nk_label_colored(ctx, u8"W,A,S,D", NK_TEXT_ALIGN_CENTERED, nk_rgb(200, 30, 0));
            nk_label_colored(ctx, u8"вверх, влево, вниз, вправо", NK_TEXT_ALIGN_CENTERED, nk_rgb(255, 255, 206));

            nk_layout_row(ctx, NK_DYNAMIC, 25, 2, ratio_one);
            nk_label_colored(ctx, u8"Shift", NK_TEXT_ALIGN_CENTERED, nk_rgb(200, 30, 0));
            nk_label_colored(ctx, u8"ускорить движение камеры", NK_TEXT_ALIGN_CENTERED, nk_rgb(255, 255, 206));

            nk_layout_row(ctx, NK_DYNAMIC, 25, 2, ratio_one);
            nk_label_colored(ctx, u8"Колёсико", NK_TEXT_ALIGN_CENTERED, nk_rgb(200, 30, 0));
            nk_label_colored(ctx, u8"приблизить/отдалить", NK_TEXT_ALIGN_CENTERED, nk_rgb(255, 255, 206));

        }
        nk_end(ctx);

        nkc_render_gui(&nkcHandle);
        time->endFrame();
    }

    time->~Time();
    nkc_shutdown(&nkcHandle);
    return 0;
}
