#pragma once

#include "glad.h"
#include <glm/glm.hpp>

#include "window.h"

struct MousePositions {
    double x;
    double y;
};

enum Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
    public:
        Camera(glm::vec3& position, glm::vec3& up, float yaw, float pitch);
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
        void processKeyboardInput(Movement direction, float deltaTime);
        void processMouseMovement(float xoffset, float yoffset, GLboolean constraintPitch = true);
        void processScrollMovement(float yoffset);
        void useKeyboardActions(GLFWwindow* window, float dt);
        void useMouseActions(const MousePositions& mousePositions);

        //  NOTE: Getter/Setter Functions
        glm::vec3 getCameraPosition() { return m_position; }
        glm::vec3 getCameraUpPosition() { return m_up; }
        float getCameraYaw() { return m_yaw; }
        float getCameraPitch() { return m_pitch; }
        float getMouseSpeed() { return m_mouseSpeed; }
        float getCameraZoom() { return Window::m_zoom; }
        float getMouseSensitivity() { return m_mouseSensitivity; }
        glm::vec3 getFrontDirection();
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix(float aspectRatio);
        
    private:
        glm::vec3 m_position;
        glm::vec3 m_up;
        float m_yaw = -90.0f;
        float m_pitch = 0.0f;

        float m_mouseSpeed = 2.5f;
        float m_mouseSensitivity = 0.006f;
        
        float near = 0.1f;
        float far = 100.0f;

        bool firstMouse = true;
        double lastX;
        double lastY;

};
