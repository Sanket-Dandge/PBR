#include "specularmap.h"
#include "constants.h"
#include "cube.h"
#include "glad.h"
#include "quad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

SpecularMap::SpecularMap(const uint32_t envCubemapID) : m_environmentCubemapId(envCubemapID) {
    // pre-filtered env map
    std::string prefilteredEnvMapVertexShaderPath = "resources/Shaders/specular.vs";
    std::string prefilteredEnvMapFragmentShaderPath = "/src/ibl/shaders/specular.fs";

    prefilteredEnvMapShader = std::make_unique<Shader>(prefilteredEnvMapVertexShaderPath.c_str(), prefilteredEnvMapFragmentShaderPath.c_str());
    prefilteredEnvMapFramebuffer = std::make_unique<MipmapCubemap>(m_prefilteredEnvMapWidth, m_prefilteredEnvMapHeight);

    // BRDF convolution
    std::string brdfConvolutionVertexShaderPath = "resources/Shaders/brdf.vs";
    std::string brdfConvolutionFragmentShaderPath = "resources/Shaders/brdf.fs";

    m_brdfConvolutionShader = std::make_unique<Shader>(brdfConvolutionVertexShaderPath.c_str(), brdfConvolutionFragmentShaderPath.c_str());
    m_brdfConvolutionFramebuffer = std::make_unique<BRDF>(m_brdfConvolutionMapWidth, m_brdfConvolutionMapHeight);
}

void SpecularMap::computePrefilteredEnvMap()
{
    // Timer timer;

    glm::mat4 model = identity;
    glm::mat4 cameraAngles[] =
        {
            glm::lookAt(origin, unitX, -unitY),
            glm::lookAt(origin, -unitX, -unitY),
            glm::lookAt(origin, unitY, unitZ),
            glm::lookAt(origin, -unitY, -unitZ),
            glm::lookAt(origin, unitZ, -unitY),
            glm::lookAt(origin, -unitZ, -unitY)
        };
    glm::mat4 projection = glm::perspective( glm::radians(90.0f), 1.0f, 0.1f, 2.0f);

    auto cube = Cube();
    prefilteredEnvMapFramebuffer->bind();
    prefilteredEnvMapShader->use();
    prefilteredEnvMapShader->setInt("environmentCubemap", 0);

    for (uint32_t mipLevel = 0; mipLevel < m_prefilteredEnvMapMipLevels; mipLevel++) {
        prefilteredEnvMapFramebuffer->setMiplevel(mipLevel);

        glViewport(0, 0, prefilteredEnvMapFramebuffer->getMipWidth(), prefilteredEnvMapFramebuffer->getMipHeight());

        // each mip level has increasing roughness
        float roughness = (float) mipLevel / (float) (m_prefilteredEnvMapMipLevels - 1);
        prefilteredEnvMapShader->setFloat("roughness", roughness);

        // render to each side of the cubemap
        for (auto i = 0; i < 6; i++) {
            prefilteredEnvMapShader->setMat4("model", model);
            prefilteredEnvMapShader->setMat4("view", cameraAngles[i]);
            prefilteredEnvMapShader->setMat4("projection", projection);
            prefilteredEnvMapFramebuffer->setCubeFace(i);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindTexture(GL_TEXTURE_CUBE_MAP, m_environmentCubemapId);
            cube.Draw();
        }
    }

    // timer.logDifference("Rendered specular pre-filtered environment map");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int SpecularMap::getPrefilteredEnvMapId()
{
   return prefilteredEnvMapFramebuffer->getCubemapID();
}

void SpecularMap::computeBrdfConvolutionMap()
{
    // Timer timer;

    auto quad = Quad();
    m_brdfConvolutionFramebuffer->bind();
    m_brdfConvolutionShader->use();

    glViewport(0, 0, m_brdfConvolutionMapWidth, m_brdfConvolutionMapHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    quad.Draw();

    // timer.logDifference("Rendered specular brdf convolution map");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int SpecularMap::getBrdfConvolutionMapId()
{
    return m_brdfConvolutionFramebuffer->getTextureColorID();
}
