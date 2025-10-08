#pragma once

#include <cstdint>

class MipmapCubemap {
    public:
        MipmapCubemap(uint32_t width, uint32_t height);
        void bind();
        void loadMipmapCubemapFramebuffer(uint32_t width, uint32_t height);
        void setMiplevel(uint32_t level);

        uint32_t getMipWidth() { return m_mipWidth; }
        uint32_t getMipHeight() { return m_mipHeight; }

        void setCubeFace(uint32_t index);
        uint32_t getCubemapID() { return m_cubemapID; }

    private:
        uint32_t m_width, m_height;
        uint32_t m_mipWidth, m_mipHeight;
        uint32_t m_mipLevel;

        uint32_t m_framebufferID;
        uint32_t m_depthRenderbufferID;
        uint32_t m_cubemapID;
};
