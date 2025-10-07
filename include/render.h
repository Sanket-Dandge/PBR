#pragma once

#include "config.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "cameraControl.h"
#include "skybox.h"
#include "shader.h"
#include "window.h"

struct Scene {
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
        ConfigInfo m_config;
        std::shared_ptr<Window> s_window;
        std::shared_ptr<CameraController> s_camera;
        std::unique_ptr<Skybox> m_skybox;
        std::unique_ptr<Cube> m_cube;
        std::unique_ptr<Scene> m_scene;

        // std::unique_ptr<Framebuffer> m_framebuffer;

        std::unique_ptr<Shader> m_cubeShader;
        std::unique_ptr<Shader> m_skyboxShader;
};
