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

    [[nodiscard]] inline const Character &GetCharacter(GLchar character) { return characters[character]; }
    [[nodiscard]] inline GLuint GetTexture(const std::string &name) { return textures[name]; }
    [[nodiscard]] inline const Shader &GetShader(const std::string &name) { return shaders[name]; }

    void LoadTexture(const std::string &name, const std::string &path, int mode);
    void LoadShader(const std::string &name, const std::string &path);
    void LoadFont(const std::string &path);

private:
    std::map<GLchar, Character> characters;
    std::map<std::string, GLuint> textures;
    std::map<std::string, Shader> shaders;
};

#endif
