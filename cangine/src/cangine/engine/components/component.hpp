// component.hpp
#pragma once

#include "../../platform/mouse.hpp"

namespace cangine {
    class GameObject;

    /**
     * @brief Base interface for all components attached to a @ref GameObject.
     *
     * Components implement behavior via lifecycle hooks: @ref firstFrame is
     * called once before the first update, @ref frame is called every frame,
     * and @ref onMouseEvent receives mouse input. Derived components override
     * the hooks they need.
     */
    class IComponent {
    private:
        GameObject* gameObject; ///< Owning game object.
        friend class GameObject;

    public:
        bool isUnique = true; ///< If true, only one of this component type may exist per object.

        /**
         * @brief Constructs the component, optionally bound to a game object.
         * @param gameObject Owning game object, or @c nullptr if unset.
         */
        IComponent(GameObject* gameObject = nullptr);
        virtual ~IComponent();

        /**
         * @brief Returns the game object this component is attached to.
         * @return Owning game object, or @c nullptr.
         */
        GameObject* getGameObject() const;

        /// @brief Called once before the first @ref frame update.
        virtual void firstFrame() { };
        /**
         * @brief Called every frame to update the component.
         * @param deltaTime Seconds elapsed since the previous frame.
         */
        virtual void frame(double deltaTime) { };
        /**
         * @brief Called when a mouse event is dispatched to the owning object.
         * @param e The mouse event.
         */
        virtual void onMouseEvent(const MouseEvent* e) { };
    };
}
