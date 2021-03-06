#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader {
private:
    GLuint id = 0;

    static inline GLuint currentUse = 0;

public:
    Shader() = default;
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    Shader(const Shader &shader) = delete;
    ~Shader();

    Shader &operator=(const Shader &shader) = delete;

    bool Load(const std::string &vertexPath, const std::string &fragmentPath);
    bool LoadSource(const std::string &vertexSrc, const std::string &fragmentSrc);

    [[nodiscard]] bool IsValid() const noexcept { return id != 0; }

    void Use() const;

    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;

    void SetVec2(const std::string &name, const glm::vec2 &value) const;
    void SetVec2(const std::string &name, float x, float y) const;

    void SetVec3(const std::string &name, const glm::vec3 &value) const;
    void SetVec3(const std::string &name, float x, float y, float z) const;

    void SetVec4(const std::string &name, const glm::vec4 &value) const;
    void SetVec4(const std::string &name, float x, float y, float z, float w) const;

    void SetMat2(const std::string &name, const glm::mat2 &mat) const;
    void SetMat3(const std::string &name, const glm::mat3 &mat) const;
    void SetMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    static bool checkCompileErrors(GLuint shader, const std::string &type);
};

#endif
