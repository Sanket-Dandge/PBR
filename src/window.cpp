#include "window.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>

float Window::m_zoom = 45.0f;

void Window::startWindow() {
    if (!glfwInit()) {
        std::runtime_error("Failed to initialize glfw");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, windowInfo.versionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, windowInfo.versionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, windowInfo.profile);

    m_window = glfwCreateWindow(windowInfo.width, windowInfo.height, windowInfo.title, NULL, NULL);
    if (m_window == NULL) {
        std::runtime_error("Failed to create window");
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::runtime_error("Failed to initialize glad");
    }

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    glfwSetScrollCallback(m_window, scroll_callback);

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::update() {
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    windowInfo.width = static_cast<uint32_t>(w);
    windowInfo.height = static_cast<uint32_t>(h);
}

void Window::shutWindow() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    processScrollMovement(static_cast<float>(yoffset));
}

void Window::processScrollMovement(float yoffset) {
    m_zoom -= yoffset;
    m_zoom = glm::clamp(m_zoom, 1.0f, 90.0f);
}

void Window::showCursor() {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::hideCursor() {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
