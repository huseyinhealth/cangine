#pragma once

#include <vector>

#include "../../gameobjects/camera_object.hpp"
#include "../quadrenderer_component.hpp"

/**
 * Panel offsets it's position to children.
 */

namespace cangine {
    class Panel;

    /// @brief A child object of a panel together with its offset from the panel.
    struct PanelChild {
        GameObject* go;  ///< The child game object.
        Vector3 offset;  ///< Offset of the child relative to the panel's position.
    };

    /**
     * @brief Container component that positions child objects relative to itself.
     *
     * Draws a panel rectangle (via @ref QuadRendererComponent) and keeps each
     * registered child positioned at its stored offset, so moving the panel
     * moves its children.
     */
    class PanelComponent : public QuadRendererComponent {
        private:
        std::vector<PanelChild*> children; ///< Registered children and their offsets.
        Vector3 pos;                       ///< Panel position used as the offset origin.
        friend class Panel;

        public:
        float padding = 5;   ///< Inner padding in pixels.
        float width = 350;   ///< Panel width in pixels.
        float height = 150;  ///< Panel height in pixels.
        Color color = Color(53, 53, 53, 1); ///< Panel background color.

        /**
         * @brief Registers a child at a zero offset.
         * @param childIn Child game object to add.
         */
        void registerChild(GameObject* childIn);
        /**
         * @brief Registers a child with a predefined offset.
         * @param childIn Child descriptor (object + offset) to add.
         */
        void registerChild(PanelChild* childIn);
        void firstFrame() override;
        void frame (double _dt) override;
        void setRectColor(Color colorIn) override;
        void setRectWidth(int widthIn) override;
        void setRectHeight(int heightIn) override;
        /**
         * @brief Sets the offset for a child object.
         * @param gameobject Child object to reposition.
         * @param offsetIn New offset relative to the panel.
         */
        void setOffset(GameObject* gameobject, Vector3 offsetIn);
        /**
         * @brief Sets the offset for a child descriptor.
         * @param panelChild Child descriptor to reposition.
         * @param offsetIn New offset relative to the panel.
         */
        void setOffset(PanelChild* panelChild, Vector3 offsetIn);
        /**
         * @brief Finds the child descriptor for a game object.
         * @param gameObject Object to look up.
         * @return The matching @ref PanelChild, or @c nullptr if not a child.
         */
        PanelChild* getPanelObject(GameObject* gameObject);
        /**
         * @brief Computes a child's world position (panel position + offset).
         * @param gameObject Child object to query.
         * @return The child's world position.
         */
        Vector3 getWorldPosition(GameObject* gameObject);
        /**
         * @brief Returns a child's offset relative to the panel.
         * @param go Child object to query.
         * @return The child's offset.
         */
        Vector3 getOffset(GameObject* go);
        ~PanelComponent();
    };
}
