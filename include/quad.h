#pragma once

#include <cstdint>
#include "buffer.h"

class Quad : public Buffer {
    public:
        Quad();

        virtual const std::vector<float>& getVertices() const override { return m_vertices; }
        const uint32_t& getFramebufferWidth() const override;
        const uint32_t& getFramebufferHeight() const override;
        void Draw();

    private:
        uint32_t m_VAO, m_VBO;
        std::vector<float> m_vertices = {
            // positions   // textureCoordinates
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };
};
