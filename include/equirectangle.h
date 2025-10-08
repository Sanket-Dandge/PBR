#pragma once

#include "cubemapBuffer.h"
#include "hdrcube.h"
#include "shader.h"
#include <cstdint>
#include <string>

class EquirectangleCubemap {
    public:
        EquirectangleCubemap(const std::string& hdrPath);
        void compute();
        uint32_t getCubemapID() { return m_framebuffer->getCubemapID(); }

    private:
        const uint32_t m_cubemapWidth = 512;
        const uint32_t m_cubemapHeight = 512;

        std::unique_ptr<Shader> m_hdrShader;
        std::unique_ptr<HDRCube> m_hdrCube;
        std::unique_ptr<CubemapBuffer> m_framebuffer;

};
