#include <imgui.h>
#include "Render.h"

Render::Render(int width, int height, ResourceManager* tx)
{
    this->projMat = glm::ortho(0.f, width * this->Zoom + 0.f, 0.f, height * this->Zoom + 0.f, 0.1f, 100.0f);
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
}

Render::~Render()
{

}

void Render::RenderSprite(Sprite* s)
{
    const Shader &ts = this->texman->GetShader(s->Shader);

    ts.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, s->Position);
    model = glm::rotate(model, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, glm::vec3(s->Scale.x, s->Scale.y, 1));

    ts.SetMat4("model", model);
    ts.SetMat4("view", this->camActive.GetViewMatrix());
    ts.SetMat4("projection", this->projMat);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texman->GetTexture(s->Texture));

    glBindVertexArray(this->boxVAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Render::RenderTextSprite(TextSprite* s, bool hd)
{
    glm::vec4 position = projMat * camActive.GetViewMatrix() * glm::vec4(s->Position.x, s->Position.y, s->Position.z, 1.0f);

    auto size = ImGui::GetWindowSize();

    auto textSize = ImGui::CalcTextSize(s->Text.c_str());

    ImGui::SetCursorPos({position.x * size.x / 2 + size.x / 2 - textSize.x / 2, size.y - (position.y * size.y / 2 + size.y / 2) - textSize.y / 2});
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(s->Color.r, s->Color.g, s->Color.b, 1.0f));
    ImGui::Text(s->Text.c_str());
    ImGui::PopStyleColor();
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
    this->projMat = glm::ortho(0.f, w * this->Zoom + 0.f, 0.f, h * this->Zoom + 0.f, 0.1f, 100.0f);
}
