#pragma once

#include "bloom.h"
#include "config.h"
#include "entity.h"
#include "equirectangle.h"
#include "framebuffer.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "cameraControl.h"
#include "irradiance.h"
#include "quad.h"
#include "skybox.h"
#include "shader.h"
#include "specularmap.h"
#include "window.h"

const int TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP = 10;
const int TEXTURE_UNIT_PREFILTERED_ENV_MAP = 11;
const int TEXTURE_UNIT_BRDF_CONVOLUTION_MAP = 12;

enum BloomDirection {
    BOTH = 0,
    HORIZONTAL = 1,
    VERTICAL = 2
};

struct Scene {
    std::vector<Entity> m_entities;
    std::vector<glm::vec3> mLightPositions;
    std::vector<glm::vec3> mLightColors;
};

class Render {
    public:
        Render(ConfigInfo &config, std::shared_ptr<Window> window, std::shared_ptr<CameraController> camera);
        void begin(std::shared_ptr<Scene> scene);
        void beginImGui();
        void shutdown();
        void render();

    private:
        void renderBloom();

        ConfigInfo m_config;
        std::shared_ptr<Window> s_window;
        std::shared_ptr<CameraController> s_camera;
        std::unique_ptr<Skybox> m_skybox;
        std::unique_ptr<Cube> m_cube;
        std::shared_ptr<Scene> m_scene;

        std::unique_ptr<Framebuffer> m_framebuffer;
        std::unique_ptr<Bloom> mBloomFramebuffers[2];
        unsigned int mBloomFramebufferResult;

        // pre-computed IBL stuff
        std::unique_ptr<EquirectangleCubemap> mIblEquirectangularCubemap;
        std::unique_ptr<DiffuseIrradianceMap> mIblDiffuseIrradianceMap;
        std::unique_ptr<SpecularMap> mIblSpecularMap;

        std::unique_ptr<Shader> m_PbrShader;
        std::unique_ptr<Shader> m_BloomShader;
        std::unique_ptr<Shader> m_PostShader;
        std::unique_ptr<Shader> m_cubeShader;
        std::unique_ptr<Shader> m_skyboxShader;

        // post-processing
        bool mBloomEnabled = true;
        float mBloomIntensity = 1.0;
        int mBloomIterations = 10;
        int mBloomDirection = BloomDirection::BOTH;
        bool mTonemappingEnabled = false;
        float mGammaCorrectionFactor = 2.2;
        float mBloomBrightnessCutoff = 1.0; // fragments with brightness above this are blurred
        std::unique_ptr<Quad> m_fullscreenQuad;
};
