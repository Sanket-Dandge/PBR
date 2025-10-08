#include "entity.h"
#include <memory>

Entity::Entity(std::shared_ptr<Model> model) : m_model(model) {}

void Entity::setPosition(glm::vec3 position) {
    m_position = position;
}

void Entity::setScale(glm::vec3 scale) {
    m_scale = scale;
}

void Entity::setOrientation(glm::quat orientation) {
    m_orientation = orientation;
}
