#pragma once

#include <functional>
#include <string>
#include <vector>

#include "./label_object.hpp"
#include "../../../platform/mouse.hpp"
#include "../../../render/texture.hpp"

namespace cangine {
    class ButtonComponent;
    /**
     * @brief Clickable button game object with a text label.
     *
     * Extends @ref Label with a @ref ButtonComponent for hover/press visuals
     * and lets callers register click handlers.
     */
    class Button : public Label {
        private:
        std::vector<std::function<void(const MouseEvent*)>> onClickHandlers; ///< Registered click callbacks.
        ButtonComponent* btnComponent; ///< Component driving button behavior.

        /**
         * @brief Invokes all registered click handlers.
         * @param e Mouse event that triggered the click.
         */
        void onClick(const MouseEvent* e);
        friend class ButtonComponent;
        public:
        /**
         * @brief Constructs a button.
         * @param text Button label text.
         * @param position Position of the button (z is the layer).
         * @param foreColor Label text color.
         * @param hoverColor Background color while hovered.
         * @param focusColor Background color while pressed.
         * @param backgroundColor Default background color.
         */
        Button(
            std::string text,
            Vector3 position      = Vector3(0, 0, 0),
            Color foreColor       = Colors::BLACK,
            Color hoverColor      = Colors::GRAY,
            Color focusColor      = Colors::DARK_GRAY,
            Color backgroundColor = Colors::WHITE
        );

        /**
         * @brief Registers a callback invoked when the button is clicked.
         * @param handler Click handler to add.
         */
        inline void registerOnClickEvent(std::function<void(const MouseEvent*)> handler) {
            onClickHandlers.push_back(handler);
        }

        /**
         * @brief Sets the button's background texture.
         * @param textureIn Texture to use.
         */
        void setTexture(Texture* textureIn);
        /**
         * @brief Returns the button's background texture.
         * @return The current texture.
         */
        Texture* getTexture();
    };
}
