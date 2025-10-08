#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3& position, glm::vec3& up, float yaw, float pitch)
    : m_position(position), m_up(up), m_yaw(yaw), m_pitch(pitch) {}


Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : m_position(posX, posY, posZ), m_up(upX, upY, upZ), m_yaw(yaw), m_pitch(pitch) {}

glm::vec3 Camera::getFrontDirection() {
    float xDirection = cos(m_yaw) * cos(m_pitch);
    float yDirection = sin(m_pitch);
    float zDirection = sin(m_yaw) * cos(m_pitch);

    return glm::normalize(glm::vec3(xDirection, yDirection, -zDirection));
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(m_position, m_position + getFrontDirection(), m_up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) {
    return glm::perspective(glm::radians(Window::m_zoom), aspectRatio, near, far);
}

void Camera::processKeyboardInput(Movement direction, float deltaTime) {
    float velocity    = m_mouseSpeed * deltaTime;
    glm::vec3 front = getFrontDirection();
    glm::vec3 right = glm::normalize(glm::cross(front, m_up));
    glm::vec3 up = glm::normalize(glm::cross(right, front));

    switch (direction) {
        case Movement::FORWARD : m_position += front * velocity; break;
        case Movement::BACKWARD : m_position -= front * velocity; break;
        case Movement::LEFT : m_position -= right * velocity; break;
        case Movement::RIGHT : m_position += right * velocity; break;
        case Movement::UP : m_position += up * velocity; break;
        case Movement::DOWN : m_position -= up * velocity; break;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constraintPitch /* = true */) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw   += xoffset;
    m_pitch += yoffset;

    // INFO: make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constraintPitch) {
        if (m_pitch > 89.0f) m_pitch = 89.0f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;
    }

}

void Camera::useKeyboardActions(GLFWwindow* window, float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)    == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
    if (glfwGetKey(window, GLFW_KEY_W)         == GLFW_PRESS) { processKeyboardInput(FORWARD, dt); }
    if (glfwGetKey(window, GLFW_KEY_S)         == GLFW_PRESS) { processKeyboardInput(BACKWARD, dt); }
    if (glfwGetKey(window, GLFW_KEY_A)         == GLFW_PRESS) { processKeyboardInput(LEFT, dt); }
    if (glfwGetKey(window, GLFW_KEY_D)         == GLFW_PRESS) { processKeyboardInput(RIGHT, dt); }
    if (glfwGetKey(window, GLFW_KEY_UP)        == GLFW_PRESS) { processKeyboardInput(UP, dt); }
    if (glfwGetKey(window, GLFW_KEY_DOWN)      == GLFW_PRESS) { processKeyboardInput(DOWN, dt); }
}

void Camera::useMouseActions(const MousePositions& mousePositions) {
    if (firstMouse) {
        lastX = mousePositions.x;
        lastY = mousePositions.y;
        firstMouse = false;
    }

    float xoffset = lastX - mousePositions.x;
    float yoffset = lastY - mousePositions.y;

    lastX = mousePositions.x;
    lastY = mousePositions.y;

    processMouseMovement(xoffset, yoffset, false);
}
