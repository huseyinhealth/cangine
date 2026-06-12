#pragma once

#include <vector>
#include <cassert>

namespace cangine {
    struct MouseEvent;
    class GameObject;
    class Window;

    /**
     * @brief Singleton registry that owns and updates all game objects.
     *
     * Tracks every registered @ref GameObject, updates them each frame and
     * forwards mouse events to them.
     */
    class ObjectManager {
        private:
            std::vector<GameObject*> objects; ///< All registered game objects.
            static ObjectManager* instance;   ///< Singleton instance.
            Window* win;                       ///< Window the manager is associated with.
        public:
            /// @brief Constructs the object manager.
            ObjectManager();
            /// @brief Destroys the object manager.
            ~ObjectManager();

            /**
             * @brief Returns the singleton instance.
             * @return Pointer to the ObjectManager instance.
             */
            static ObjectManager* getInstance();
            /**
             * @brief Registers a game object for updates and event delivery.
             * @param obj Object to register.
             */
            void registerObject(GameObject* obj);
            /// @brief Updates every registered game object for the current frame.
            void updateAll();
            /**
             * @brief Forwards a mouse event to all registered objects.
             * @param event Mouse event to dispatch.
             */
            void fireMouseEvent(const MouseEvent* event);
    };
}
