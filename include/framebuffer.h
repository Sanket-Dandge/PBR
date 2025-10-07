#pragma once

#include "buffer.h"
#include <cstdint>

class Framebuffer : public Buffer {
    public:
        Framebuffer(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}
        void resize(uint32_t width, uint32_t height);
        void bind();
        const std::vector<float>& getVertices() const override;
        const uint32_t& getFramebufferWidth() const override { return m_width; }
        const uint32_t& getFramebufferHeight() const override { return m_height; }

    private:
        uint32_t m_width;
        uint32_t m_height;
};

