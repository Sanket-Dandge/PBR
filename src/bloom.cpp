#include "bloom.h"
#include "glad.h"
#include <cmath>
#include <cstddef>

Bloom::Bloom(int width, int height) : m_width(width), m_height(height) {}

void Bloom::loadBloomFramebuffer() {
    glGenFramebuffers(1, &m_framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

    // create a color texture
    glGenTextures(1, &m_textureColorID);
    glBindTexture(GL_TEXTURE_2D, m_textureColorID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);

    // attach the color texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorID, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Bloom::bind() {
    int width = m_width / std::pow(2, m_mipLevel);
    int height = m_height / std::pow(2, m_mipLevel);

    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
}

void Bloom::resize(int width, int height) {
    m_width = width;
    m_height = height;

    glBindTexture(GL_TEXTURE_2D, m_textureColorID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Bloom::setMipLevel(int mipLevel) {
    m_mipLevel = mipLevel;
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorID, mipLevel);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
