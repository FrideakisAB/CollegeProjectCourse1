#include "Render/Shader.h"

#include "Log.h"
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    Load(vertexPath, fragmentPath);
}

Shader::~Shader()
{
    if (id != 0)
        glDeleteProgram(id);
}

bool Shader::Load(const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexCode, fragmentCode;
    std::ifstream vertexShaderFile, fragmentShaderFile;

    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);
        std::stringstream vertexStream, fragmentStream;

        vertexStream << vertexShaderFile.rdbuf();
        fragmentStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode   = vertexStream.str();
        fragmentCode = fragmentStream.str();
    }
    catch (const std::ifstream::failure &e)
    {
        Log::Get().Error(std::string("Shader file read error: ") + e.what());
        return false;
    }

    return LoadSource(vertexCode, fragmentCode);
}

bool Shader::LoadSource(const std::string &vertexSrc, const std::string &fragmentSrc)
{
    bool isCorrect = true;

    GLuint vertex, fragment;

    const char *vertexSrcC = vertexSrc.c_str();
    const char *fragmentSrcC = fragmentSrc.c_str();

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSrcC, nullptr);
    glCompileShader(vertex);
    if (!checkCompileErrors(vertex, "Vertex"))
        isCorrect = false;

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSrcC, nullptr);
    glCompileShader(fragment);
    if (!checkCompileErrors(fragment, "Fragment"))
        isCorrect = false;

    if (id != 0)
        glDeleteProgram(id);

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    if (!checkCompileErrors(id, "Program"))
        isCorrect = false;

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if (!isCorrect)
    {
        glDeleteProgram(id);
        id = 0;
    }

    return isCorrect;
}

void Shader::Use() const
{
    if (currentUse != id)
    {
        glUseProgram(id);
        currentUse = id;
    }
}

void Shader::SetBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::SetVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::SetVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::SetVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

bool Shader::checkCompileErrors(GLuint shader, const std::string &type)
{
    int success;
    char infoLog[1024];
    if (type != "Program")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            Log::Get().Error(std::string("Shader compilation error ") + type);
            Log::Get().Info(std::string("Shader compilation error, description: ") + infoLog);

            return false;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            Log::Get().Error(std::string("Program linking error ") + type);
            Log::Get().Info(std::string("Program linking error, description: ") + infoLog);

            return false;
        }
    }

    return true;
}
