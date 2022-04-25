#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include <cwchar>
#include <iostream>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"
#include <ft2build.h>
#include <glm/glm.hpp>
#include FT_FREETYPE_H

struct Character {
    GLuint     TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint     Advance;
};

class ResourceManager {
public:
    ResourceManager();
    virtual ~ResourceManager();

    [[nodiscard]] GLuint GetSprite(const std::string& name);
    [[nodiscard]] Shader *GetShader(const std::string& name);

    void LoadTexture(const std::string& name, const std::string& path, int mode);
    void LoadShader(const std::string& name, const std::string& path);
    void LoadFont(const std::string& path);

    std::map<GLchar, Character> Characters;

private:
    std::map<std::string, GLuint> textures;
    std::map<std::string, Shader*> shaders;
};

#endif
