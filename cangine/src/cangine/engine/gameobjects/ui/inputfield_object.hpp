#pragma once

#include <functional>

#include "../game_object.hpp"
#include "../../components/ui/inputfield_component.hpp"

namespace cangine {
    /**
     * @brief Editable text-field game object.
     *
     * A thin convenience wrapper around @ref InputFieldComponent, exposing
     * getters and setters for the field's content, style and the on-return
     * callback so it can be configured without touching the component.
     */
    class InputField : public GameObject {
        private:
        InputFieldComponent* ifc; ///< Component implementing the input field.

        public:
        /// @brief Constructs an input field with a default style.
        InputField();
        /**
         * @brief Sets the field's text content and refreshes the view.
         * @param contentIn New content string.
         */
        void setContent(std::string contentIn) {
            ifc->content = contentIn;
            ifc->updateView();
        }
        /**
         * @brief Sets the placeholder text shown when empty.
         * @param placeholderIn New placeholder string.
         */
        void setPlaceholder(std::string placeholderIn) { ifc->placeholder = placeholderIn; }
        /**
         * @brief Sets the caret position and refreshes the view.
         * @param posIn New caret index into the content.
         */
        void setCursorPos(int posIn) {
            ifc->cursorpos = posIn;
            ifc->updateView();
        }
        /**
         * @brief Sets the field width.
         * @param widthIn New width in pixels.
         */
        void setWidth(float widthIn) { ifc->width = (int)widthIn; }
        /**
         * @brief Sets the field height (and scales the font to match).
         * @param heightIn New height in pixels.
         */
        void setHeight(float heightIn) { ifc->height = (int)heightIn; ifc->setFontHeight((heightIn * 4) / 5); }
        /**
         * @brief Sets the border thickness.
         * @param widthIn New border width in pixels.
         */
        void setBorderWidth(float widthIn) { ifc->borderWidth = widthIn; }
        /**
         * @brief Sets the placeholder text color.
         * @param colorIn New color.
         */
        void setPlaceHolderColor(Color colorIn) { ifc->placeholderColor = colorIn; }
        /**
         * @brief Sets the border color when unfocused.
         * @param colorIn New color.
         */
        void setBorderColor(Color colorIn) { ifc->borderColor = colorIn; }
        /**
         * @brief Sets the border color when focused.
         * @param colorIn New color.
         */
        void setBorderColorFocus(Color colorIn) { ifc->borderColorFocus = colorIn; }
        /**
         * @brief Sets the caret color.
         * @param colorIn New color.
         */
        void setCursorColor(Color colorIn) { ifc->cursorColor = colorIn; }
        /**
         * @brief Sets the field background color.
         * @param colorIn New color.
         */
        void setBackgroundColor(Color colorIn) { ifc->backgroundColor = colorIn; }
        /**
         * @brief Sets the text color.
         * @param colorIn New color.
         */
        void setColor(Color colorIn) { ifc->color = colorIn; }
        /**
         * @brief Sets the callback invoked when Enter is pressed.
         * @param functionIn Callback receiving the field content.
         */
        void setOnReturn(std::function<void(std::string)> functionIn) { ifc->onReturn = functionIn; }

        /**
         * @brief Returns the underlying input field component.
         * @return The @ref InputFieldComponent.
         */
        InputFieldComponent* getIFC() { return ifc; }
        /**
         * @brief Returns the current content.
         * @return Reference to the content string.
         */
        const std::string&   getContent() const { return ifc->content; }
        /**
         * @brief Returns the placeholder text.
         * @return Reference to the placeholder string.
         */
        const std::string&   getPlaceholder() const { return ifc->placeholder; }
        /**
         * @brief Returns the caret position.
         * @return Caret index into the content.
         */
        int                  getCursorPos() const { return ifc->cursorpos; }
        /**
         * @brief Returns the field width.
         * @return Width in pixels.
         */
        float                getWidth() const { return (float)ifc->width; }
        /**
         * @brief Returns the field height.
         * @return Height in pixels.
         */
        float                getHeight() const { return (float)ifc->height; }
        /**
         * @brief Returns the border thickness.
         * @return Border width in pixels.
         */
        float                getBorderWidth() const { return ifc->borderWidth; }
        /**
         * @brief Returns the placeholder text color.
         * @return Reference to the placeholder color.
         */
        const Color&         getPlaceholderColor() const { return ifc->placeholderColor; }
        /**
         * @brief Returns the unfocused border color.
         * @return Reference to the border color.
         */
        const Color&         getBorderColor() const { return ifc->borderColor; }
        /**
         * @brief Returns the focused border color.
         * @return Reference to the focused border color.
         */
        const Color&         getBorderColorFocus() const { return ifc->borderColorFocus; }
        /**
         * @brief Returns the caret color.
         * @return Reference to the caret color.
         */
        const Color&         getCursorColor() const { return ifc->cursorColor; }
        /**
         * @brief Returns the background color.
         * @return Reference to the background color.
         */
        const Color&         getBackgroundColor() const { return ifc->backgroundColor; }
        /**
         * @brief Returns the text color.
         * @return Reference to the text color.
         */
        const Color&         getColor() const { return ifc->color; }
    };
}
