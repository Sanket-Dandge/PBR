#include "irradiance.h"

#include "glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "constants.h"
#include "cube.h"
#include "timer.h"
#include "cubemapBuffer.h"

DiffuseIrradianceMap::DiffuseIrradianceMap(const unsigned int environmentCubemapId) : m_envCubemapID(environmentCubemapId) {
    std::string diffuseIrradianceVertexShaderPath = "resources/Shaders/irradiance.vs";
    std::string diffuseIrradianceFragmentShaderPath = "/src/ibl/shaders/irradiance.fs";

    diffuseIrradianceShader = std::make_unique<Shader>(diffuseIrradianceVertexShaderPath.c_str(), diffuseIrradianceFragmentShaderPath.c_str());
    diffuseIrradianceFramebuffer = std::make_unique<CubemapBuffer>(diffuseIrradianceMapWidth, diffuseIrradianceMapHeight);
}

void DiffuseIrradianceMap::compute() {
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
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 2.0f);

    auto cube = Cube();
    glViewport(0, 0, diffuseIrradianceMapWidth, diffuseIrradianceMapHeight);
    diffuseIrradianceFramebuffer->bind();
    diffuseIrradianceShader->use();

    // render to each side of the cubemap
    for (auto i = 0; i < 6; i++) {
        diffuseIrradianceShader->setMat4("model", model);
        diffuseIrradianceShader->setMat4("view", cameraAngles[i]);
        diffuseIrradianceShader->setMat4("model", projection);
        diffuseIrradianceFramebuffer->setCubeFace(i);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        diffuseIrradianceShader->setInt("environmentCubemap", 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemapID);
        cube.Draw();
    }

    // timer.logDifference("Rendered diffuse irradiance map");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t DiffuseIrradianceMap::getCubemapID() {
   return diffuseIrradianceFramebuffer->getCubemapID();
}
