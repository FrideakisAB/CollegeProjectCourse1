#include "Render/SpriteTask.h"

#include "Engine.h"
#include "Game/Sprite.h"

SpriteTask::SpriteTask(const Sprite &sprite)
{
    ShaderName = sprite.Shader;
    Texture = sprite.Texture;

    Position = sprite.Position;
    Scale = sprite.Scale;
}

void SpriteTask::initializeMesh()
{
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
    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(boxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteTask::Draw(const Shader &shader)
{
    if (boxVAO == 0)
        initializeMesh();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Engine::Get().GetResourceManager().GetTexture(Texture.data()));

    glBindVertexArray(boxVAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}
