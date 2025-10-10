#pragma once

#include "config.h"
#include "camera.h"
#include "window.h"
#include <memory>

class CameraController {
    public:
        CameraController(ConfigInfo& config, std::shared_ptr<Window> window) : m_config(config), s_window(window) {}
        void begin();
        void update(float dt);
        void drawDebugPanel();

        glm::vec3 getCameraPosition();
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix(float aspectRatio);

    private:
        ConfigInfo m_config;
        bool mouseEnabled = true;
        MousePositions m_mousePosition;
        std::shared_ptr<Window> s_window;
        std::unique_ptr<Camera> m_camera;
};
