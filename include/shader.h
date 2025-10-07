#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <glm/glm.hpp>

class Shader {
    public:
        uint32_t ID;
        Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

        std::string readFile(const std::filesystem::path& path) const;
        void checkCompileErrors(const char* path, unsigned int index, const std::string& type);
        void use();

        void setBool(const std::string& name, bool value) const;
        void setFloat(const std::string& name, float value) const;
        void setInt(const std::string& name, int value) const;

        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec2(const std::string& name, float x, float y) const;

        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec3(const std::string& name, float x, float y, float z) const;

        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setVec4(const std::string& name, float x, float y, float z, float w) const;

        void setMat2(const std::string& name, const glm::mat2& value) const;
        void setMat3(const std::string& name, const glm::mat3& value) const;
        void setMat4(const std::string& name, const glm::mat4& value) const;

};
