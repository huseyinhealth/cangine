#include "camera_object.hpp"

#include "../components/camera_component.hpp"

namespace cangine {
    
    Camera::Camera() : GameObject() {
        CameraComponent* cc = new CameraComponent();
        this->appendComponent <CameraComponent> (cc);
    }
    
}