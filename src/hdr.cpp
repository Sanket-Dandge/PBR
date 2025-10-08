#include "hdr.h"
#include "Core.h"
#include "glad.h"
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

HDR::HDR(const std::string& path) {
    loadHDR(path);
}

void HDR::loadHDR(const std::string& path) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        LOG("Failed to load HDR Texture: %s", path.c_str());
        stbi_image_free(data);
        return;
    }

    glGenTextures(1, &m_hdrID);
    glBindTexture(GL_TEXTURE_2D, m_hdrID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
}
