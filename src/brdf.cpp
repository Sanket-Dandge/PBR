#include "brdf.h"
#include "glad.h"

BRDF::BRDF(uint32_t width, uint32_t height) : m_width(width), m_height(height) {
    brdfFramebuffer(width, height);
}

void BRDF::brdfFramebuffer(uint32_t width, uint32_t height) {
    // framebuffer
    glGenFramebuffers(1, &m_framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

    // depth buffer
    glGenRenderbuffers(1, &m_depthRenderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    // attach the depth buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderbufferID);

    // color texture
    glGenTextures(1, &m_textureColorID);
    glBindTexture(GL_TEXTURE_2D, m_textureColorID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorID, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BRDF::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
}
