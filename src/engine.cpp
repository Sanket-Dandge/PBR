#include "engine.h"
#include "Core.h"
#include <GLFW/glfw3.h>
#include "render.h"
#include <memory>

Engine::Engine(ConfigInfo& config) : config(config) {
    s_window = std::make_shared<Window>();
    s_camera = std::make_shared<CameraController>(config, s_window);
    s_render = std::make_shared<Render>(config, s_window, s_camera);
}

void Engine::begin(std::shared_ptr<Scene> scene) {
    LOG("Starting Engine");
    s_window->startWindow();
    LOG("Starting Rendering");
    s_render->begin(scene);
    LOG("After Rendering");
    s_camera->begin();
}

void Engine::run() {
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    while (!s_window->shouldWindowClose()) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        s_render->beginImGui();

        s_camera->update(deltaTime);
        s_render->render();
    }
}

void Engine::shutdown() {
    s_render->shutdown();
    s_window->shutWindow();
}
