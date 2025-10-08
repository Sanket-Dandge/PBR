#pragma once

#include "model.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

class Entity {
    public:
        Entity(std::shared_ptr<Model> model);
        void setPosition(glm::vec3 position);
        glm::vec3 getPosition() { return m_position; }

        void setScale(glm::vec3 scale);
        glm::vec3 getScale() { return m_scale; }

        void setOrientation(glm::quat orientation);
        glm::quat getOrientation() { return m_orientation; }
        std::shared_ptr<Model> getModel() { return m_model; }

    private:
        glm::vec3 m_position;
        glm::vec3 m_scale = glm::vec3(1.0f);
        glm::quat m_orientation;
        std::shared_ptr<Model> m_model;
        
};
