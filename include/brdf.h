#pragma once

#include <cstdint>

class BRDF {
    public:
        BRDF(uint32_t width, uint32_t height);
        void bind();
        void brdfFramebuffer(uint32_t width, uint32_t height);
        uint32_t getWidth() { return m_width; }
        uint32_t getHeight() { return m_height; }
        uint32_t getTextureColorID() { return m_textureColorID; }
    
    private:
        uint32_t m_width, m_height;
        uint32_t m_framebufferID;
        uint32_t m_depthRenderbufferID;
        uint32_t m_textureColorID;

};
