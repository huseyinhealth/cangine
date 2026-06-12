#pragma once
#include <vector>
#include <typeinfo>
#include <cassert>

#include "../components/component.hpp"
#include "../../platform/mouse.hpp"

namespace cangine {
    class ObjectManager;
    /**
     * @brief Base class for everything that lives in the scene.
     *
     * A game object is a container of @ref IComponent instances. It exposes a
     * type-safe component API (@ref appendComponent, @ref getComponent,
     * @ref newComponent, ...) and lifecycle hooks (@ref onUpdate,
     * @ref onMouseEvent, @ref onEnable, @ref onDisable) that subclasses may
     * override. Managed by the @ref ObjectManager.
     */
    class GameObject {
        private:
        std::vector<IComponent*> components; ///< Components attached to this object.
        bool pendingDestroy = false;         ///< Whether the object is queued for destruction.
        bool active = true;                  ///< Whether the object is active (updated/drawn).
        friend class ObjectManager;
        public:
        GameObject();
        virtual ~GameObject();
        /**
         * @brief Per-frame update hook.
         * @param deltaTime Seconds elapsed since the previous frame.
         */
        virtual void onUpdate(double deltaTime);
        /**
         * @brief Mouse event hook.
         * @param e The mouse event delivered to this object.
         */
        virtual void onMouseEvent(const MouseEvent* e);
        /// @brief Called when the object becomes inactive.
        virtual void onDisable() {};
        /// @brief Called when the object becomes active.
        virtual void onEnable() {};

        /// @brief Marks the object for destruction by the @ref ObjectManager.
        void destroy() {
            this->pendingDestroy = true;
        }

        /**
         * @brief Attaches an existing component to this object.
         *
         * Calls the component's @ref IComponent::firstFrame once attached.
         * Asserts if a unique component of the same type already exists.
         * @tparam T Component type.
         * @param component Component instance to attach (ownership taken).
         */
        template <typename T>
        void appendComponent(T* component) {
            if (component) {
                if (getComponent<T>() != nullptr && component->isUnique) {
                    assert(false && "Unique component already exist in gameobject");
                    return; 
                }

                component->gameObject = this;
                components.push_back(component);

                component->firstFrame();
            }
        }
        
        /**
         * @brief Destroys and removes the first component of a type.
         * @tparam T Component type to remove.
         */
        template <typename T>
        void destroyComponent() {
            for (auto it = components.begin(); it != components.end(); ++it) {
                if (dynamic_cast<T*>(*it)) {
                    delete *it;
                    components.erase(it);
                    return;
                }
            }
        }

        /**
         * @brief Returns the first attached component of a type.
         * @tparam T Component type to find.
         * @return Pointer to the component, or @c nullptr if none.
         */
        template <typename T>
        T* getComponent() {
            for (auto* component : components) {
                T* target = dynamic_cast<T*>(component);
                if (target != nullptr) {
                    return target;
                }
            }
            return nullptr;
        }

        /**
         * @brief Returns all attached components of a type.
         * @tparam T Component type to find.
         * @return Vector of matching components (possibly empty).
         */
        template <typename T>
        std::vector<T*> getComponents() {
            std::vector<T*> result;
            for (auto* component : components) {
                T* target = dynamic_cast<T*>(component);
                if (target != nullptr) {
                    result.push_back(target);
                }
            }
            return result;
        }

        /**
         * @brief Constructs a component in place and attaches it.
         * @tparam T Component type to create.
         * @tparam Args Constructor argument types.
         * @param args Arguments forwarded to the component's constructor.
         * @return Pointer to the newly created component.
         */
        template <typename T, typename... Args>
        T* newComponent(Args&&... args) {
            T* component = new T(std::forward<Args>(args)...);
            this->appendComponent<T>(component);
            return component;
        }

        /**
         * @brief Activates or deactivates the object.
         * @param a @c true to enable, @c false to disable.
         */
        void setActive(bool a);
        /**
         * @brief Reports whether the object is active.
         * @return @c true if active.
         */
        bool isActive();
    };
}
