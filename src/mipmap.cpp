#include "mipmap.h"
#include "glad.h"
#include <cmath>
#include <cstddef>

MipmapCubemap::MipmapCubemap(uint32_t width, uint32_t height) : m_width(width), m_height(height), m_mipLevel(0) {
    loadMipmapCubemapFramebuffer(width, height);
}

void MipmapCubemap::loadMipmapCubemapFramebuffer(uint32_t width, uint32_t height) {
    // framebuffer
    glGenFramebuffers(1, &m_framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

    // depth buffer
    glGenRenderbuffers(1, &m_depthRenderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    // attach the depth buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderbufferID);

    // cubemap
    glGenTextures(1, &m_cubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapID);

    // specify/allocate each face for the cubemap
    for (auto i = 0; i < 6;  i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // trilinear filtering for the mipmap
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MipmapCubemap::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
}

void MipmapCubemap::setMiplevel(uint32_t level) {
    m_mipLevel = level;
    m_mipWidth = m_width * std::pow(0.5, m_mipLevel);
    m_mipHeight = m_height * std::pow(0.5, m_mipLevel);

    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_mipWidth, m_mipHeight);
}

void MipmapCubemap::setCubeFace(unsigned int faceIndex) {
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, m_cubemapID, m_mipLevel);
}
