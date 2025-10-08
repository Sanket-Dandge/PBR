#include "cubemapBuffer.h"
#include "glad.h"
#include <cstddef>

CubemapBuffer::CubemapBuffer(int width, int height) {
    loadCubemapFramebuffer(width, height);
}

void CubemapBuffer::loadCubemapFramebuffer(int width, int height) {
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
        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubemapBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
}

void CubemapBuffer::generateMipmap() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void CubemapBuffer::setCubeFace(uint32_t index) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, m_cubemapID, 0);
}

uint32_t CubemapBuffer::getCubemapID() {
    return m_cubemapID;
}
