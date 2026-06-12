#pragma once

#include <unordered_set>

#include "../game_object.hpp"
#include "../../components/ui/panel_component.hpp"

namespace cangine {
    /**
     * @brief Container game object that groups and positions child objects.
     *
     * Backed by a @ref PanelComponent: children are placed relative to the
     * panel, and enabling/disabling the panel cascades to its children.
     */
    class Panel : public GameObject {
        private:
        PanelComponent* pc = nullptr;            ///< Component implementing panel layout.
        std::unordered_set<PanelChild*> notActive; ///< Children that were inactive before the panel was disabled.

       public:
        /**
         * @brief Constructs a panel.
         * @param padding Inner padding in pixels.
         */
        Panel(int padding = 5);
        void onDisable() override;
        void onEnable() override;
        /**
         * @brief Sets the inner padding.
         * @param paddingIn New padding in pixels.
         */
        void setPadding(int paddingIn);
        /**
         * @brief Adds a child object to the panel.
         * @param childIn Child game object to add.
         */
        void registerChild(GameObject *childIn);
        /**
         * @brief Sets the panel background color.
         * @param colorIn New color.
         */
        void setRectColor(Color colorIn);
        /**
         * @brief Sets the panel width.
         * @param widthIn New width in pixels.
         */
        void setRectWidth(int widthIn);
        /**
         * @brief Sets the panel height.
         * @param heightIn New height in pixels.
         */
        void setRectHeight(int heightIn);
        /**
         * @brief Sets a child's offset relative to the panel.
         * @param go Child object to reposition.
         * @param offset New offset.
         */
        void setOffset(GameObject* go, Vector3 offset);

        /**
         * @brief Returns a child's offset relative to the panel.
         * @param go Child object to query.
         * @return The child's offset.
         */
        Vector3 getOffset(GameObject* go);
        /**
         * @brief Returns the panel's size.
         * @return Width and height as a vector.
         */
        Vector2 getRectSize();
    };
}
