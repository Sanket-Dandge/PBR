#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <glm/glm.hpp>

struct Texture {
    uint32_t m_textureID;
    std::string m_filePath;
};

struct Material {
    bool useTextureAlbedo = false;
    bool useTextureMetallicRoughness = false;
    bool useTextureNormal = false;
    bool useTextureAmbientOcclusion = false;
    bool useTextureEmissive = false;

    glm::vec3 albedo = glm::vec3(1.0f, 0.0f, 0.0f);
    float metallic = 1.0f;
    float roughness = 0.0f;
    float ambientOcclusion = 1.0f;
    glm::vec3 emissive = glm::vec3(0.0f, 0.0f, 0.0f);

    std::shared_ptr<Texture> textureAlbedo;
    std::shared_ptr<Texture> textureMetallicRoughness;
    std::shared_ptr<Texture> textureNormal;
    std::shared_ptr<Texture> textureAmbientOcclusion;
    std::shared_ptr<Texture> textureEmissive;
};
