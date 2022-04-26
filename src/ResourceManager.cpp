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
	LoadShader("Text", "Text");
    LoadFont("resources/Arial_AMU.ttf");
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

void ResourceManager::LoadFont(const std::string& path)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }
    FT_Face face;
    if (FT_New_Face(ft, path.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        characters.insert(std::pair<GLchar, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
