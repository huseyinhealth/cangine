// IComponent.cpp
#include "component.hpp"

namespace cangine {
    IComponent::IComponent(GameObject *gameObject) {
        this->gameObject = gameObject;
    }

    IComponent::~IComponent() = default;

    GameObject* IComponent::getGameObject() const {
        return gameObject;
    }

}
