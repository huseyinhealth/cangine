#pragma once

#include <string>

#include "component.hpp"
#include "../../render/renderer/text_renderer.hpp"
#include "../../util/color.hpp"
#include "../../shapes/shapes.hpp"
#include "../../render/texture.hpp"

namespace cangine {
    /**
     * @brief Component that renders a text string, optionally over a background.
     *
     * Loads a font, draws @ref text each frame via the @ref TextRenderer, and
     * can paint a colored or textured background rectangle behind the text.
     */
    class TextRendererComponent : public IComponent {
        protected:
        TextRenderer* tr;            ///< Renderer used to draw the text.
        std::string path;            ///< Path of the loaded font file.
        int fontHeight;              ///< Pixel height the font is loaded at.
        Font* font = nullptr;        ///< Loaded font, or @c nullptr.
        Texture* backTex = nullptr;  ///< Optional background texture.

        public:
        Color color;             ///< Text color.
        Color backgroundColor;   ///< Background fill color.
        float backgroundPadding; ///< Padding between text and background edges.
        float scale;             ///< Scale factor applied to the text.
        std::string text;        ///< The string to render.
        Rectangle rect;          ///< Background rectangle geometry.

        void firstFrame() override;
        void frame(double dt) override;

        /**
         * @brief Returns the rendered size of the current text.
         * @return Width and height the text occupies.
         */
        Vector2 getSize();

        /**
         * @brief Sets the font's pixel height (reloads the font).
         * @param heightIn New pixel height.
         */
        void setFontHeight(int heightIn);
        /**
         * @brief Returns the current font pixel height.
         * @return Font height in pixels.
         */
        int getFontHeight();

        /**
         * @brief Sets the background texture drawn behind the text.
         * @param textureIn Texture to use, or @c nullptr for none.
         */
        void setBackTexture(Texture* textureIn);
        /**
         * @brief Returns the background texture.
         * @return Background texture, or @c nullptr if unset.
         */
        Texture* getBackTexture() const;
    };
}