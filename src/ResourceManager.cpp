#include "ResourceManager.h"

#include <GL/glew.h>

ResourceManager::ResourceManager()
{
	LoadTexture("Ball", "resources/textures/ball.png", 4);
	LoadTexture("PBall", "resources/textures/pball.png", 4);
	LoadTexture("ArrowStart", "resources/textures/arrowStart.png", 4);
	LoadTexture("ArrowXBody", "resources/textures/arrowXBody.png", 4);
	LoadTexture("ArrowYBody", "resources/textures/arrowYBody.png", 4);
	LoadTexture("ArrowXEnd", "resources/textures/arrowXEnd.png", 4);
	LoadTexture("ArrowYEnd", "resources/textures/arrowYEnd.png", 4);
	LoadShader("Standard", "Standard");
}

void ResourceManager::LoadTexture(const std::string& name, const std::string& path, int mode)
{
    int w, h, comp;
    unsigned char* image = stbi_load(path.c_str(), &w, &h, &comp, mode);

    if (image == nullptr)
        return;

    glGenTextures(1, &textures[name]);

    glBindTexture(GL_TEXTURE_2D, textures[name]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (comp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    else if (comp == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);
}

void ResourceManager::LoadShader(const std::string& name, const std::string& path)
{
    if (!shaders[name].Load("resources/shaders/" + path + "Vertex.glsl", "resources/shaders/" + path + "Fragment.glsl"))
        shaders.erase(shaders.find(name));
}
