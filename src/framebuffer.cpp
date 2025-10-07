#include "framebuffer.h"
#include "glad.h"

void Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void Framebuffer::resize(uint32_t width, uint32_t height) {
    glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, m_textureBloom);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

const std::vector<float>& Framebuffer::getVertices() const {
    return {};
}
