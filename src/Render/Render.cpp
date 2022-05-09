#include "Render/Render.h"

#include <imgui.h>
#include "Engine.h"

Render::Render(int width, int height) :
    width(width), height(height)
{
    ProjectMat = glm::ortho(0.f, width * Zoom + 0.f, 0.f, height * Zoom + 0.f, 0.1f, 100.0f);

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

Render::~Render()
{
    glDeleteVertexArrays(1, &boxVAO);
}

void Render::RenderSprite(const Sprite &sprite) const
{
    const Shader &shader = Engine::Get().GetResourceManager().GetShader(sprite.Shader);

    shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, sprite.Position);
    model = glm::rotate(model, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, glm::vec3(sprite.Scale.x, sprite.Scale.y, 1));

    shader.SetMat4("model", model);
    shader.SetMat4("view", Camera::Main.value().get().GetViewMatrix());
    shader.SetMat4("projection", ProjectMat);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Engine::Get().GetResourceManager().GetTexture(sprite.Texture));

    glBindVertexArray(boxVAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Render::RenderTextSprite(const TextSprite &sprite) const
{
    glm::vec4 position = ProjectMat * Camera::Main.value().get().GetViewMatrix() * glm::vec4(sprite.Position.x, sprite.Position.y, sprite.Position.z, 1.0f);

    auto size = ImGui::GetWindowSize();

    auto textSize = ImGui::CalcTextSize(sprite.Text.c_str());

    glm::vec2 screenPosition = glm::vec2(position.x * size.x / 2 + size.x / 2 - textSize.x / 2, size.y - (position.y * size.y / 2 + size.y / 2) - textSize.y / 2);

    screenPosition.x = glm::clamp(screenPosition.x, textSize.x / 2, size.x - textSize.x);
    screenPosition.y = glm::clamp(screenPosition.y, textSize.y / 2, size.y - textSize.y);

    ImGui::SetCursorPos({screenPosition.x , screenPosition.y});
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(sprite.Color.r, sprite.Color.g, sprite.Color.b, 1.0f));
    ImGui::Text(sprite.Text.c_str());
    ImGui::PopStyleColor();
}

void Render::ResizeWindow(int width, int height)
{
    this->width = width;
    this->height = height;
    ProjectMat = glm::ortho(0.f, width * Zoom + 0.f, 0.f, height * Zoom + 0.f, 0.1f, 100.0f);
}
