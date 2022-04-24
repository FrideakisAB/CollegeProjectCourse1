#include "Render.h"

Render::Render(int width, int height, TextureManager* tx)
{
    this->projMat = glm::ortho( 0.f, width * this->Zoom + 0.f, 0.f, height * this->Zoom + 0.f, 0.1f, 100.0f);
    this->texman = tx;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
        // positions    // texture coords
         0.5f,  0.5f,   1.0f, 1.0f, // top right
         0.5f, -0.5f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, EBO;
    glGenVertexArrays(1, &this->boxVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(this->boxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &tVAO);
    glGenBuffers(1, &tVBO);
    glBindVertexArray(tVAO);
    glBindBuffer(GL_ARRAY_BUFFER, tVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Render::~Render()
{
    //dtor
}

void Render::RenderSprite(Sprite* s)
{
    Shader* ts = this->texman->getShader(s->getShader());

    ts->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(s->x, s->y, s->z));
    model = glm::rotate(model, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, glm::vec3(s->sx, s->sy, 1));

    ts->setMat4("model", model);
    ts->setMat4("view", this->camActive.GetViewMatrix());
    ts->setMat4("projection", this->projMat);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texman->getSprite(s->getTexure()));

    glBindVertexArray(this->boxVAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Render::RenderTextSprite(TextSprite* s, bool hd = false)
{
    Shader* ts = this->texman->getShader("Text");
    float scale = 0.7f;

    ts->use();

    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(s->x - 114, s->y - 104, s->z));
    //model = glm::rotate(model, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
    //model = glm::scale(model, glm::vec3(s->sx, s->sy, 1));

    ts->setMat4("model", model);
    ts->setMat4("view", this->camActive.GetViewMatrix());
    ts->setMat4("projection", this->projMat);

    ts->setVec3("textColor", s->color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->tVAO);

    std::string text = s->getText();
    float x = s->x;
    float y = s->y;
    float eps = 0;
    if(hd)
        x -= 7.4f * text.size();
    else
        x -= 3 * text.size();
    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = this->texman->Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos, ypos + h, 0.0, 0.0 },
            { xpos, ypos, 0.0, 1.0 },
            { xpos + w, ypos, 1.0, 1.0 },
            { xpos, ypos + h, 0.0, 0.0 },
            { xpos + w, ypos, 1.0, 1.0 },
            { xpos + w, ypos + h, 1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, tVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Render::setCam(Camera cam)
{
    this->camActive = cam;
}

Camera* Render::getCam()
{
    return &this->camActive;
}

void Render::resizeW(int w, int h)
{
    this->projMat = glm::ortho( 0.f, w * this->Zoom + 0.f, 0.f, h * this->Zoom + 0.f, 0.1f, 100.0f);
}
