#include "render.h"
#include "config.h"
#include "glad.h"
#include "model.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Render::Render(ConfigInfo &config, std::shared_ptr<Window> window, std::shared_ptr<CameraController> camera) 
    : m_config(config), s_window(window), s_camera(camera) {}

void Render::begin(std::shared_ptr<Scene> scene) {
    m_scene = scene;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    stbi_set_flip_vertically_on_load(true);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = m_config.imguiFontScale;

    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(s_window->getGLFWwindow(), true);
    ImGui_ImplOpenGL3_Init(m_config.glslVersion);

    //  INFO: Framebuffer Loading

    m_framebuffer = std::make_unique<Framebuffer>(s_window->getWidth(), s_window->getHeight());
    m_framebuffer->init();

    mBloomFramebuffers[0] = std::make_unique<Bloom>(s_window->getWidth(), s_window->getHeight());
    mBloomFramebuffers[0]->loadBloomFramebuffer();
    mBloomFramebuffers[1] = std::make_unique<Bloom>(s_window->getWidth(), s_window->getHeight());
    mBloomFramebuffers[1]->loadBloomFramebuffer();

    //  INFO: Shader Loading
    m_cubeShader = std::make_unique<Shader>("resources/Shaders/cube.vs", "resources/Shaders/cube.fs");
    m_skyboxShader = std::make_unique<Shader>("resources/Shaders/skybox.vs", "resources/Shaders/skybox.fs");
    m_PbrShader = std::make_unique<Shader>("resources/Shaders/pbr.vs", "resources/Shaders/pbr.fs");
    m_BloomShader = std::make_unique<Shader>("resources/Shaders/bloom.vs", "resources/Shaders/bloom.fs");
    m_PostShader = std::make_unique<Shader>("resources/Shaders/post.vs", "resources/Shaders/post.fs");

    // Pre-compute IBL stuff
    mIblEquirectangularCubemap = std::make_unique<EquirectangleCubemap>(m_config.hdrPath);
    mIblEquirectangularCubemap->compute();

    mIblDiffuseIrradianceMap = std::make_unique<DiffuseIrradianceMap>(mIblEquirectangularCubemap->getCubemapID());
    mIblDiffuseIrradianceMap->compute();

    mIblSpecularMap = std::make_unique<SpecularMap>(mIblEquirectangularCubemap->getCubemapID());
    mIblSpecularMap->computePrefilteredEnvMap();
    mIblSpecularMap->computeBrdfConvolutionMap();

    // glViewport(0, 0, s_window->getWidth(), s_window->getHeight()); // set initial viewport size

    std::vector<std::string> faces = {
        "resources/Textures/skybox/right.jpg",
        "resources/Textures/skybox/left.jpg",
        "resources/Textures/skybox/top.jpg",
        "resources/Textures/skybox/bottom.jpg",
        "resources/Textures/skybox/front.jpg",
        "resources/Textures/skybox/back.jpg"
    };

    m_fullscreenQuad = std::make_unique<Quad>();
    m_skybox = std::make_unique<Skybox>(mIblEquirectangularCubemap->getCubemapID());
}

void Render::shutdown() {}

void Render::beginImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Engine");
}

void Render::renderBloom() {
    // Bloom pass
    glm::vec2 blurDirectionX = glm::vec2(1.0f, 0.0f);
    glm::vec2 blurDirectionY = glm::vec2(0.0f, 1.0f);

    switch (mBloomDirection) {
        case BloomDirection::HORIZONTAL:
            blurDirectionY = blurDirectionX;
            break;
        case BloomDirection::VERTICAL:
            blurDirectionX = blurDirectionY;
        default:
            break;
    }

    glBindTexture(GL_TEXTURE_2D, m_framebuffer->getBloomColorTextureId());
    glGenerateMipmap(GL_TEXTURE_2D);

    m_BloomShader->use();

    for (auto mipLevel = 0; mipLevel <= 5; mipLevel++) {
        mBloomFramebuffers[0]->setMipLevel(mipLevel);
        mBloomFramebuffers[1]->setMipLevel(mipLevel);

        // first iteration we use the bloom buffer from the main render pass
        mBloomFramebuffers[0]->bind();
        glBindTexture(GL_TEXTURE_2D, m_framebuffer->getBloomColorTextureId());
        m_BloomShader->setInt("sampleMipLevel", mipLevel);
        m_BloomShader->setVec2("blurDirection", blurDirectionX);

        m_fullscreenQuad->Draw();

        uint32_t bloomFramebuffer = 1; // which buffer to use

        for (auto i = 1; i < mBloomIterations; i++) {
            unsigned int sourceBuffer = bloomFramebuffer == 1 ? 0 : 1;
            mBloomFramebuffers[bloomFramebuffer]->bind();
            auto blurDirection = bloomFramebuffer == 1 ? blurDirectionY : blurDirectionX;
            m_BloomShader->setVec2("blurDirection", blurDirection);
            glBindTexture(GL_TEXTURE_2D, mBloomFramebuffers[sourceBuffer]->getColorTextureId());
            m_fullscreenQuad->Draw();
            bloomFramebuffer = sourceBuffer;
        }

        mBloomFramebufferResult = bloomFramebuffer;
    }
}

void Render::render() {
    if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Average FPS %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    s_camera->drawDebugPanel();

    if (ImGui::CollapsingHeader("Post-processing", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Bloom (Gaussian)");
        ImGui::Checkbox("Enabled", &mBloomEnabled);
        ImGui::SliderFloat("Intensity", &mBloomIntensity, 0.0, 5.0);
        ImGui::SliderFloat("Threshold", &mBloomBrightnessCutoff, 0.01, 5.0);
        ImGui::SliderInt("Blur Iterations", &mBloomIterations, 2, 20);
        ImGui::Text("Direction: "); ImGui::SameLine();
        ImGui::RadioButton("Both", &mBloomDirection, 0); ImGui::SameLine();
        ImGui::RadioButton("Horizontal", &mBloomDirection, 1); ImGui::SameLine();
        ImGui::RadioButton("Vertical", &mBloomDirection, 2);

        ImGui::Text("Post");
        ImGui::Checkbox("HDR Tone Mapping (Reinhard)", &mTonemappingEnabled);
        ImGui::SliderFloat("Gamma Correction", &mGammaCorrectionFactor, 1.0, 3.0);
    }

    m_framebuffer->bind();
    // glViewport(0, 0, s_window->getWidth(), s_window->getHeight());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 camPosition = s_camera->getCameraPosition();
    glm::mat4 projection = s_camera->getProjectionMatrix((float)s_window->getWidth() / (float)s_window->getHeight());
    glm::mat4 view = s_camera->getViewMatrix();

    m_PbrShader->use();

    m_PbrShader->setVec3Array("lightPositions", m_scene->m_lightPositions);
    m_PbrShader->setVec3Array("lightColors", m_scene->m_lightColors);
    m_PbrShader->setVec3("cameraPosition", camPosition);

    // IBL stuff
    glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
    m_PbrShader->setInt("diffuseIrradianceMap", TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mIblDiffuseIrradianceMap->getCubemapID());

    glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_PREFILTERED_ENV_MAP);
    m_PbrShader->setInt("prefilteredEnvMap", TEXTURE_UNIT_PREFILTERED_ENV_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mIblSpecularMap->getPrefilteredEnvMapId());

    glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
    m_PbrShader->setInt("brdfConvolutionMap", TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
    glBindTexture(GL_TEXTURE_2D, mIblSpecularMap->getBrdfConvolutionMapId());

    // post stuff for main shader
    m_PbrShader->setFloat("bloomBrightnessCutoff", mBloomBrightnessCutoff);

    for (auto entity : m_scene->m_entities) {
        glm::mat4 model = glm::mat4(1.0f);

        auto rotationMatrix = glm::toMat4(entity.getOrientation());
        model = rotationMatrix * model;

        model = glm::translate(model, entity.getPosition());
        model = glm::scale(model, entity.getScale());

        m_PbrShader->setMat4("model", model);
        m_PbrShader->setMat4("view", view);
        m_PbrShader->setMat4("projection", projection);

        entity.getModel()->Draw(*m_PbrShader);
    }


    // skybox (draw this last to avoid running fragment shader in places where objects are present
    m_skyboxShader->use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); // remove translation so skybox is always surrounding camera

    m_skyboxShader->setInt("skybox", 0); // set skybox sampler to slot 0
    m_skyboxShader->setMat4("model", model);
    m_skyboxShader->setMat4("view", skyboxView);
    m_skyboxShader->setMat4("projection", projection);
    m_skyboxShader->setFloat("bloomBrightnessCutoff", mBloomBrightnessCutoff);
    m_skybox->Draw();

    renderBloom();

    // Post-processing pass
    glViewport(0, 0, s_window->getWidth(), s_window->getHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // switch back to default fb
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_PostShader->use();

    m_PostShader->setBool("bloomEnabled", mBloomEnabled);
    m_PostShader->setFloat("bloomIntensity", mBloomIntensity);
    m_PostShader->setBool("tonemappingEnabled", mTonemappingEnabled);
    m_PostShader->setFloat("gammaCorrectionFactor", mGammaCorrectionFactor);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_framebuffer->getColorTextureId());
    m_PostShader->setInt("colorTexture", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mBloomFramebuffers[mBloomFramebufferResult]->getColorTextureId());
    m_PostShader->setInt("bloomTexture", 1);

    m_fullscreenQuad->Draw();

    // draw ImGui
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(s_window->getGLFWwindow());
    glfwPollEvents();
}
