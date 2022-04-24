#ifndef CANVAS_H
#define CANVAS_H

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Layout.h"
#include "Element.h"
#include "TextureManager.h"
#include "ButtonUI.h"
#include "ImageUI.h"
#include "ProgresBarUI.h"
#include "TextUI.h"
#include "TextDynUI.h"

class Canvas
{
public:
    Canvas(TextureManager* tm, int width, int height);
    virtual ~Canvas();

    void addLayout(string root, string name, glm::vec4 sz);
    void addLayout(string root, string name, float x, float y, float rx, float by);

    void addElement(string root, ButtonUI elm);
    void addElement(string root, ImageUI elm);
    void addStaticElement(string root, TextUI elm);
    TextUI* addElement(string root, TextUI elm);
    TextDynUI* addElement(string root, TextDynUI elm);

    TextDynUI* GetElement(string nam);

    ProgresBarUI* addElement(string root, ProgresBarUI elm);

    void Draw(Render* ren, GLFWwindow* window);
    void preBuild();

    //TEST
    Layout* getLayout(string name);
    //TEST

private:
    vector<ProgresBarUI> pbb;
    vector<ButtonUI> butt;
    vector<ImageUI> imgg;
    vector<TextUI> txtt;
    vector<TextDynUI> txtd;
    vector<Element*> elms;
    vector<Layout> lts;

    TextureManager* tm;
    int width, height;
};

#endif // CANVAS_H
