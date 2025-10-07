#pragma once

#include <cstdint>
#include <vector>

class Buffer {
    public:
        void loadBuffer();
        void loadFramebuffer();
        virtual const std::vector<float>& getVertices() const = 0;
        virtual const uint32_t& getFramebufferWidth() const = 0;
        virtual const uint32_t& getFramebufferHeight() const = 0;
        uint32_t getFramebufferID() { return m_framebuffer; }
        uint32_t getColorBuffer() { return m_textureColorBuffer; }
        uint32_t getBloomTexture() { return m_textureBloom; }

        uint32_t m_VAO, m_VBO, m_framebuffer, m_textureColorBuffer, m_textureBloom, m_depthRenderbuffer;
};
