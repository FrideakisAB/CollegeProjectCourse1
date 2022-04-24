#include "TextureManager.h"

TextureManager::TextureManager()
{
	LoadTexture("Ball", "resources/textures/ball.png", 4);
	LoadTexture("PBall", "resources/textures/pball.png", 4);
	LoadTexture("ArrowStart", "resources/textures/arrowStart.png", 4);
	LoadTexture("ArrowXBody", "resources/textures/arrowXBody.png", 4);
	LoadTexture("ArrowYBody", "resources/textures/arrowYBody.png", 4);
	LoadTexture("ArrowXEnd", "resources/textures/arrowXEnd.png", 4);
	LoadTexture("ArrowYEnd", "resources/textures/arrowYEnd.png", 4);
	LoadShader("Standart", "Standart");
	LoadShader("Text", "Text");
    LoadFont("resources/Arial_AMU.ttf");
}

TextureManager::~TextureManager()
{

}

void TextureManager::LoadTexture(string n, string p, int mode)
{
    int w;
    int h;
    int comp;
    unsigned char* image = stbi_load(p.c_str(), &w, &h, &comp, mode);

    if(image == nullptr)
        throw(std::string("Failed to load texture"));

    glGenTextures(1, &this->spriteT[n]);

    glBindTexture(GL_TEXTURE_2D, this->spriteT[n]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(comp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    else if(comp == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);
}

void TextureManager::LoadShader(string n, string pe)
{
    this->shaders[n] = new Shader(string("resources/shaders/" + pe + "Vertex.glsl").c_str(), string("resources/shaders/" + pe + "Fragment.glsl").c_str());
}

void TextureManager::LoadShader(string n, string p1, string p2)
{
    this->shaders[n] = new Shader(string("resources/shaders/" + p1 + "Vertex.glsl").c_str(), string("resources/shaders/" + p2 + "Fragment.glsl").c_str());
}

void TextureManager::LoadFont(string path)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    FT_Face face;
    if (FT_New_Face(ft, path.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
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
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        this->Characters.insert(std::pair<GLchar, Character>(c, character));
        // Characters[c] = character;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

GLuint TextureManager::getSprite(string name)
{
    return this->spriteT[name];
}

Shader* TextureManager::getShader(string name)
{
    return this->shaders[name];
}
