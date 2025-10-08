#include "glad.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

#include "Core.h"
#include "shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
    std::string vertexCode = readFile(vertexShaderPath);
    std::string fragmentCode = readFile(fragmentShaderPath);

    const char* vertexShaderCode = vertexCode.c_str();
    const char* fragmentShaderCode = fragmentCode.c_str();

    uint32_t vertexIndex, fragmentIndex;

    vertexIndex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexIndex, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexIndex);
    checkCompileErrors(vertexShaderPath, vertexIndex, "COMPILE");

    fragmentIndex = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentIndex, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentIndex);
    checkCompileErrors(fragmentShaderPath, fragmentIndex, "COMPILE");

    ID = glCreateProgram();
    glAttachShader(ID, vertexIndex);
    glAttachShader(ID, fragmentIndex);
    glLinkProgram(ID);
    checkCompileErrors("Shader Program Link", ID, "LINK");

    glDeleteShader(vertexIndex);
    glDeleteShader(fragmentIndex);
}

std::string Shader::readFile(const std::filesystem::path& path) const {
    std::ifstream file(path);
    if (!file.is_open()) {
        LOG("Shader File Not Found: %s", path.c_str());
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void Shader::checkCompileErrors(const char* path, unsigned int index, const std::string& type) {
    int success;
    char infoLog[512];
    if (type == "COMPILE") {
        glGetShaderiv(index, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(index, 512, NULL, infoLog);
            LOG("%s: %s", path, infoLog);
        }
    } else if (type == "LINK") {
        glGetProgramiv(index, GL_LINK_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(index, 512, NULL, infoLog);
            LOG("%s: %s", path, infoLog);
        }
    }
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& value) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setVec3Array(const std::string& name, std::vector<glm::vec3> values) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), values.size(), &values[0][0]);
}
