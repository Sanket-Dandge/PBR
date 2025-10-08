#pragma once

#include <cstdint>
#include <memory>

#include "brdf.h"
#include "mipmap.h"
#include "shader.h"

class SpecularMap {
    public:
        SpecularMap(const unsigned int envCubemapID);
        void computePrefilteredEnvMap();
        unsigned int getPrefilteredEnvMapId();
        void computeBrdfConvolutionMap();
        unsigned int getBrdfConvolutionMapId();

    private:
        // prefiltered environment map
        const uint32_t m_prefilteredEnvMapMipLevels = 5;
        const uint32_t m_prefilteredEnvMapWidth = 128;
        const uint32_t m_prefilteredEnvMapHeight = 128;

        const uint32_t m_environmentCubemapId;

        std::unique_ptr<Shader> prefilteredEnvMapShader;
        std::unique_ptr<MipmapCubemap> prefilteredEnvMapFramebuffer;

        // brdf convolution
        uint32_t m_brdfConvolutionMapId;
        const uint32_t m_brdfConvolutionMapWidth = 512;
        const uint32_t m_brdfConvolutionMapHeight = 512;

        std::unique_ptr<Shader> m_brdfConvolutionShader;
        std::unique_ptr<BRDF> m_brdfConvolutionFramebuffer;
};
