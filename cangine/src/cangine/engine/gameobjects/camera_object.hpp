#pragma once
#include "game_object.hpp"

namespace cangine {
    /**
     * @brief Game object representing the scene camera.
     *
     * Comes preconfigured with a @ref CameraComponent (and a @ref Transform)
     * so its position defines the rendered viewpoint.
     */
    class Camera : public GameObject {
        public:
        /// @brief Constructs a camera with its camera/transform components.
        Camera();
    };
}