#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include <iostream>
#include "Render/Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"
#include <glm/glm.hpp>

class ResourceManager {
public:
    ResourceManager();

    [[nodiscard]] inline GLuint GetTexture(const std::string &name) { return textures[name]; }
    [[nodiscard]] inline const Shader &GetShader(const std::string &name) { return shaders[name]; }

    void LoadTexture(const std::string &name, const std::string &path, int mode);
    void LoadShader(const std::string &name, const std::string &path);

private:
    std::map<std::string, GLuint> textures;
    std::map<std::string, Shader> shaders;
};

#endif
