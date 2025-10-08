#pragma once

#include <cstdint>

class CubemapBuffer {
    public:
        CubemapBuffer(int width, int height);
        void bind();
        void generateMipmap();
        void loadCubemapFramebuffer(int width, int height);

        void setCubeFace(uint32_t index);
        uint32_t getCubemapID();

    private:
        int m_width, m_height;
        uint32_t m_depthRenderbufferID;
        uint32_t m_framebufferID;
        unsigned int m_cubemapID;
};
