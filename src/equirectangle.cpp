#include "equirectangle.h"
#include "Core.h"
#include "constants.h"
#include "glad.h"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>

EquirectangleCubemap::EquirectangleCubemap(const std::string& hdrPath) {
    std::string hdrVertexShader = "resources/Shaders/hdrcube.vs";
    std::string hdrFragmentShader = "resources/Shaders/hdrcube.fs";

    m_hdrShader = std::make_unique<Shader>(hdrVertexShader.c_str(), hdrFragmentShader.c_str());
    m_hdrCube = std::make_unique<HDRCube>(hdrPath);
    m_framebuffer = std::make_unique<CubemapBuffer>(m_cubemapWidth, m_cubemapHeight);
}

void EquirectangleCubemap::compute() {
    // Timer timer();
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

    glViewport(0, 0, m_cubemapWidth, m_cubemapHeight);

    // render the equirectangular HDR texture to a cubemap
    m_framebuffer->bind();
    m_hdrShader->use();

    // render to each side of the cubemap
    for (auto i = 0; i < 6; i++) {
        m_hdrShader->setMat4("model", model);
        m_hdrShader->setMat4("view", cameraAngles[i]);
        m_hdrShader->setMat4("projection", projection);
        m_framebuffer->setCubeFace(i);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_hdrCube->Draw(*m_hdrShader);
    }

    m_framebuffer->generateMipmap();

    // timer.logDifference("Rendered equirectangular cubemap");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
