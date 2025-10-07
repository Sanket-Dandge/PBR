#include "cameraControl.h"

#include "Core.h"
#include "camera.h"
#include <GLFW/glfw3.h>
#include <memory>

void CameraController::begin() {
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);

    float yaw = 0;
    float pitch = 0;

    m_camera = std::make_unique<Camera>(position, up, yaw, pitch);
    m_mousePosition.x = static_cast<float>(s_window->getWidth() / 2.0);
    m_mousePosition.y = static_cast<float>(s_window->getHeight() / 2.0);
}

void CameraController::update(float dt) {
    double xpos, ypos;
    glfwGetCursorPos(s_window->getGLFWwindow(), &xpos, &ypos);
    m_mousePosition.x = static_cast<float>(xpos);
    m_mousePosition.y = static_cast<float>(ypos);

    m_camera->useMouseActions(m_mousePosition);
    m_camera->useKeyboardActions(s_window->getGLFWwindow(), dt);
}

glm::vec3 CameraController::getCameraPosition() {
    return m_camera->getCameraPosition();
}

glm::mat4 CameraController::getViewMatrix() {
    return m_camera->getViewMatrix();
}

glm::mat4 CameraController::getProjectionMatrix(float aspectRatio) {
    return m_camera->getProjectionMatrix(aspectRatio);
}
