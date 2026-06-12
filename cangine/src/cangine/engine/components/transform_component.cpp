#include "transform_component.hpp"


namespace cangine
{
    Transform::~Transform() = default;

    void Transform::firstFrame() {
        this->position = { 0, 0, 0 };
        this->rotation = { 0, 0, 0 };
        this->scale = { 1, 1, 1 };
    }

        void Transform::setPosition(Vector3 positionIn) {
        this->position = positionIn;
    }

    Vector3 Transform::getPosition() const {
        return this->position;
    }

    void Transform::setRotation(Vector3 rotationIn) {
        this->rotation = rotationIn;
    }

    Vector3 Transform::getRotation() const {
        return this->rotation;
    }

    void Transform::setScale(Vector3 scaleIn) {
        this->scale = scaleIn;
    }

    Vector3 Transform::getScale() const {
        return this->scale;
    }
}
