#pragma once

#include "config.h"
#include "cameraControl.h"
#include "render.h"
#include "window.h"
#include <memory>

class Engine {
    public:
        Engine(ConfigInfo& config);
        void begin(std::shared_ptr<Scene> scene);
        void run();
        void update();
        void shutdown();

    private:
        ConfigInfo& config;
        std::shared_ptr<Window> s_window;
        std::shared_ptr<CameraController> s_camera;
        std::shared_ptr<Render> s_render;

};
