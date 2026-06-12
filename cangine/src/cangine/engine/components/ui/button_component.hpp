#pragma once

#include "../component.hpp"
#include "../../../util/color.hpp"
#include "../../../render/texture.hpp"
#include "../textrenderer_component.hpp"

namespace cangine {
    class Button;
    /**
     * @brief Component providing clickable-button behavior and visuals.
     *
     * Tracks hover/press state from mouse events and swaps the background
     * color accordingly. Attaches to a @ref Button game object.
     */
    class ButtonComponent : public IComponent {
        private:
        bool mouseHover = false;   ///< Whether the cursor is over the button.
        bool mousePressed = false; ///< Whether the button is currently pressed.
        Button* button;            ///< Owning button object.
        TextRendererComponent* trc = nullptr; ///< Label text renderer, if present.

        public:
        Color hoverColor = Colors::GRAY;        ///< Background color while hovered.
        Color focusColor = Colors::DARK_GRAY;   ///< Background color while pressed.
        Color backgroundColor = Colors::WHITE;  ///< Default background color.
        void firstFrame() override;
        void onMouseEvent(const MouseEvent* e) override;

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

