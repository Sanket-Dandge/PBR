#pragma once

#include "shader.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    uint32_t id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Texture> textures;
        uint32_t VAO;

        Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
        void Draw(Shader& shader);

    private:
        uint32_t VBO, EBO;
        void setupMesh();
};
