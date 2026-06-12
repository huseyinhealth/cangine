#pragma once

#include <string>
#include <functional>

#include "../textrenderer_component.hpp"

namespace cangine {
    struct KeyEvent;
    class QuadRenderer;

    /**
     * @brief Editable single-line text input field.
     *
     * Extends @ref TextRendererComponent to capture keyboard/character input,
     * maintain a caret and content string, scroll the visible text, and fire a
     * callback when Enter is pressed. Only one field is @ref focused at a time.
     */
    class InputFieldComponent : public TextRendererComponent {
        private:
        float blinkTimer = 0;       ///< Accumulator driving the caret blink.
        size_t eventHandlerID;      ///< Id of the registered character-event handler.
        size_t keyEventHandlerID;   ///< Id of the registered key-event handler.
        size_t startIdx;            ///< Index of the first visible character (horizontal scroll).
        Transform* t;               ///< Cached transform of the owning object.
        QuadRenderer* qr;           ///< Renderer used to draw border/caret.

        /**
         * @brief Draws the field background, border, text and caret.
         * @param dt Seconds elapsed since the previous frame.
         */
        void draw(double dt);

        public:
        int width = 128; ///< Field width in pixels.
        int height = 25; ///< Field height in pixels.

        std::string content; ///< Current text content.
        int cursorpos = 0;   ///< Caret position as an index into @ref content.

        // event
        std::function<void(std::string)> onReturn = nullptr; ///< Called with the content when Enter is pressed.

        // style
        float borderWidth;        ///< Border thickness in pixels.
        std::string placeholder;  ///< Text shown when the field is empty.
        Color placeholderColor;   ///< Color of the placeholder text.
        Color borderColor;        ///< Border color when not focused.
        Color borderColorFocus;   ///< Border color when focused.
        Color cursorColor;        ///< Caret color.
        static InputFieldComponent* focused; ///< The currently focused field, or @c nullptr.

        InputFieldComponent();
        ~InputFieldComponent();
        void firstFrame() override;
        void frame(double dt) override;
        /// @brief Recomputes the visible text window from the caret position.
        void updateView();
        /**
         * @brief Inserts typed characters at the caret.
         * @param s UTF-8 text that was typed.
         */
        void inputEventHandler(std::string s);
        /**
         * @brief Handles editing keys (arrows, backspace, enter, ...).
         * @param event The key event.
         */
        void keyEventHandler(KeyEvent event);
        void onMouseEvent(const MouseEvent *e) override;
    };
}
