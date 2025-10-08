#include "quad.h"
#include "glad.h"

Quad::Quad() {
    loadQuadBuffer();
}

void Quad::Draw() {
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
}

const uint32_t& Quad::getFramebufferWidth() const {
    static const uint32_t dummyWidth = 0;
    return dummyWidth;
}

const uint32_t& Quad::getFramebufferHeight() const {
    static const uint32_t dummyHeight = 0;
    return dummyHeight;
}
