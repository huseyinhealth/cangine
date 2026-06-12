#pragma once
#include "component.hpp"

namespace cangine {

    /**
     * @brief Component marking its game object as the active camera.
     *
     * Exposes a singleton accessor so renderers can locate the current camera.
     */
    class CameraComponent : public IComponent{
        private:
        static CameraComponent* instance; ///< Singleton instance (the active camera).
        public:
        /**
         * @brief Constructs the camera component and registers it as active.
         * @param gameObject Owning game object.
         */
        CameraComponent(GameObject* gameObject = nullptr);
        /**
         * @brief Returns the active camera component.
         * @return Pointer to the active CameraComponent.
         */
        static CameraComponent* getInstance();
    };
}

