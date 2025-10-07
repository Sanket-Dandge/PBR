#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>

struct WindowInfo {
    const char* title = "Physically Based Rendering";
    uint32_t width = 3500;
    uint32_t height = 1440;
    int versionMajor = 3;
    int versionMinor = 3;
    const int profile = GLFW_OPENGL_CORE_PROFILE;
};

class Window {
    public:
        Window() {}
        void startWindow();
        void setupCallbacks(GLFWwindow* window);
        void update();

        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        void processInput(GLFWwindow* window);
        
        uint32_t getWidth() { return windowInfo.width; }
        uint32_t getHeight() { return windowInfo.height; }
        GLFWwindow* getGLFWwindow() { return m_window; }
        bool shouldWindowClose() { return glfwWindowShouldClose(m_window); }

        static void processScrollMovement(float yoffset);

        void shutWindow();
        static float m_zoom;
        
    private:
        GLFWwindow* m_window;
        WindowInfo windowInfo;
        bool firstMouse = true;
        float deltaTime = 0.0f;

        float lastX = windowInfo.width / 2.0f;
        float lastY = windowInfo.height / 2.0f;

        void onScrollMovement(double xoffset, double yoffset);
       
};
