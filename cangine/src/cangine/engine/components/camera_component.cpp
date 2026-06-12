#include "camera_component.hpp" 
#include <cassert>

/**
 * CameraComponent and CameraObject is bascially a singleton and does nothing.
 * Camera game objects adds Camera to itself automatically just to tell the engine
 * - "Hey, theres camera in the game, offset every object's position according to camera!"
 * 
 * That's basically it.
 * 
 * Actual camera behaviour is implemented in "utils/position.cpp"
 */

namespace cangine {
    CameraComponent* CameraComponent::instance = nullptr;
    CameraComponent::CameraComponent (GameObject* _gameObject) : IComponent(_gameObject) {
       assert(this->instance == nullptr && "Only one CameraComponent instance can exist!");

       this->instance = this;
    }

    CameraComponent* CameraComponent::getInstance() {
        return instance;
    }
}
