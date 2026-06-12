#pragma once

#include <string>

#include "../game_object.hpp"

#include "../../components/textrenderer_component.hpp"
#include "../../../types/vector2.hpp"
#include "../../../types/vector3.hpp"

/**
 * Label is just putting TextRendererComponent to itself but makes
 * easier to initialize it and saves you from writing more code.
 */

namespace cangine {
    /**
     * @brief Convenience game object that displays a line of text.
     *
     * Wraps a @ref TextRendererComponent so callers can create on-screen text
     * without wiring up the component manually.
     */
    class Label : public GameObject {
        private:
        TextRendererComponent* trc = nullptr; ///< The underlying text renderer component.
        public:
        /**
         * @brief Constructs a label.
         * @param text Initial text to display.
         * @param position Position of the label (z is the layer).
         */
        Label(std::string text = "Label",
            Vector3 position = Vector3(0, 0, 80));

        /**
         * @brief Returns the current text.
         * @return The displayed string.
         */
        std::string getText();
        /**
         * @brief Returns the rendered size of the text.
         * @return Width and height of the text.
         */
        Vector2 getSize();
        /**
         * @brief Returns the font pixel height.
         * @return Font height in pixels.
         */
        int getFontHeight();

        /**
         * @brief Sets the displayed text.
         * @param text New text.
         */
        void setText(std::string text);
        /**
         * @brief Sets the font pixel height.
         * @param heightIn New font height in pixels.
         */
        void setFontHeight(int heightIn);
        /**
         * @brief Sets the text color.
         * @param colorIn New text color.
         */
        void setColor(Color colorIn);
        /**
         * @brief Sets the background color behind the text.
         * @param colorIn New background color.
         */
        void setBackgroundColor(Color colorIn);

        /**
         * @brief Returns the underlying text renderer component.
         * @return The label's @ref TextRendererComponent.
         */
        TextRendererComponent* getTextRendererComponent();
    };
}
