#pragma once

#include "cubemapBuffer.h"
#include "shader.h"
#include <cstdint>

class DiffuseIrradianceMap {
    public:
        DiffuseIrradianceMap(const uint32_t envCubemapID);
        void compute();
        uint32_t getCubemapID();

    private:
        const uint32_t diffuseIrradianceMapWidth = 32;
        const uint32_t diffuseIrradianceMapHeight = 32;
        const uint32_t m_envCubemapID;

        std::unique_ptr<Shader> diffuseIrradianceShader;
        std::unique_ptr<CubemapBuffer> diffuseIrradianceFramebuffer;
};
