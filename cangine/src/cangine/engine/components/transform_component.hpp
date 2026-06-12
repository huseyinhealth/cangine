#pragma once

#include "component.hpp"
#include "../gameobjects/game_object.hpp"

#include "../../types/types.hpp"

namespace cangine {
    /**
     * @brief Component holding an object's position, rotation and scale.
     *
     * Every spatial game object carries a Transform. Convenience static
     * helpers (@ref from, @ref posFrom, @ref posFor, ...) read or write the
     * transform of a given @ref GameObject without manually fetching the
     * component.
     */
    class Transform : public IComponent {
        Vector3 position; ///< World position.
        Vector3 rotation; ///< Rotation in degrees (per axis).
        Vector3 scale;    ///< Scale factor per axis.

        public:
        ~Transform() override;
        void firstFrame() override;

        /**
         * @brief Sets the position.
         * @param positionIn New position.
         */
        void setPosition(Vector3 positionIn);
        /**
         * @brief Returns the position.
         * @return Current position.
         */
        Vector3 getPosition() const;

        /**
         * @brief Sets the rotation.
         * @param rotationIn New rotation in degrees.
         */
        void setRotation(Vector3 rotationIn);
        /**
         * @brief Returns the rotation.
         * @return Current rotation in degrees.
         */
        Vector3 getRotation() const;

        /**
         * @brief Sets the scale.
         * @param scaleIn New scale factor per axis.
         */
        void setScale(Vector3 scaleIn);
        /**
         * @brief Returns the scale.
         * @return Current scale factor per axis.
         */
        Vector3 getScale() const;

        /**
         * @brief Fetches the Transform component of a game object.
         * @param go Game object to query.
         * @return The object's Transform component.
         */
        static Transform* from(GameObject* go) {
            return go->getComponent<Transform>();
        }

        /**
         * @brief Reads the position of a game object's Transform.
         * @param go Game object to query.
         * @return The object's position.
         */
        static Vector3 posFrom(GameObject* go) {
            return go->getComponent<Transform>()->getPosition();
        }

        /**
         * @brief Reads the rotation of a game object's Transform.
         * @param go Game object to query.
         * @return The object's rotation in degrees.
         */
        static Vector3 rotFrom(GameObject* go) {
            return go->getComponent<Transform>()->getRotation();
        }

        /**
         * @brief Reads the scale of a game object's Transform.
         * @param go Game object to query.
         * @return The object's scale.
         */
        static Vector3 scaleFrom(GameObject* go) {
            return go->getComponent<Transform>()->getScale();
        }

        /**
         * @brief Sets the position on a game object's Transform.
         * @param go Game object to modify.
         * @param pos New position.
         */
        static void posFor(GameObject* go, Vector3 pos) {
            go->getComponent<Transform>()->setPosition(pos);
        }

        /**
         * @brief Sets the rotation on a game object's Transform.
         * @param go Game object to modify.
         * @param rot New rotation in degrees.
         */
        static void rotFor(GameObject* go, Vector3 rot) {
            go->getComponent<Transform>()->setRotation(rot);
        }

        /**
         * @brief Sets the scale on a game object's Transform.
         * @param go Game object to modify.
         * @param scale New scale factor per axis.
         */
        static void scaleFor(GameObject* go, Vector3 scale) {
            go->getComponent<Transform>()->setScale(scale);
        }
    };
}

