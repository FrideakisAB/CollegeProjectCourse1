#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <GL/glew.h>
#include<iostream>
#include <cwchar>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

struct Character {
    GLuint     TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint     Advance;
};

class TextureManager
{
public:
    TextureManager();
    virtual ~TextureManager();

    GLuint getSprite(string name);
    Shader* getShader(string name);

    void LoadTexture(string name, string path, int mode);
    void LoadShader(string name, string path);
    void LoadShader(string name, string p1, string p2);
    void LoadFont(string path);

    map<GLchar, Character> Characters;

private:
    map<string, GLuint> spriteT;
    map<string, Shader*> shaders;
};

#endif // TEXTUREMANAGER_H
