#include "glad.h"
#include "cube.h"

Cube::Cube() {
    loadBuffer();
}

void Cube::Draw() {
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    glBindVertexArray(0);
}

const uint32_t& Cube::getFramebufferWidth() const {
    static const uint32_t dummyWidth = 0;
    return dummyWidth;
}

const uint32_t& Cube::getFramebufferHeight() const {
    static const uint32_t dummyHeight = 0;
    return dummyHeight;
}
