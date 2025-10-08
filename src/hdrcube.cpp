#include "hdrcube.h"
#include "glad.h"
#include <memory>

HDRCube::HDRCube(const std::string& hdrPath) : m_hdr(HDR(hdrPath)) {
    m_cube = std::make_unique<Cube>();
}

void HDRCube::Draw(Shader& shader) {
    shader.setInt("hdri", 0);
    glBindTexture(GL_TEXTURE_2D, m_hdr.getID());
    m_cube->Draw();
}
