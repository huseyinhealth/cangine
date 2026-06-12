#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../../types/types.hpp"
#include "../texture.hpp"
#include "../../util/color.hpp"
#include "../../shapes/shapes.hpp"

namespace cangine {
    /**
     * @brief A single cached glyph: its texture and layout metrics.
     */
    struct Character {
        Texture texture;       ///< Texture holding the rasterized glyph.
        int width;             ///< Glyph bitmap width in pixels.
        int height;            ///< Glyph bitmap height in pixels.
        int bearingX;          ///< Horizontal offset from the origin to the glyph's left edge.
        int bearingY;          ///< Vertical offset from the baseline to the glyph's top edge.
        unsigned int advance;  ///< Horizontal advance to the next glyph (in 1/64 px).
    };

    /**
     * @brief A loaded font face plus its cache of rasterized glyphs.
     */
    struct Font {
        FT_Face face;                                    ///< Underlying FreeType face handle.
        std::unordered_map<uint32_t, Character> glyphs;  ///< Cache of glyphs keyed by codepoint.
        int height;                                      ///< Pixel height the face is set to.
        std::string path;                                ///< Path the font was loaded from.

        /**
         * @brief Constructs a font wrapper around a FreeType face.
         * @param f FreeType face handle.
         * @param h Pixel height the face is set to.
         * @param p Path the font was loaded from.
         */
        Font(FT_Face f, int h, const std::string& p)
            : face(f), height(h), path(p) {}
    };

    /**
     * @brief Decodes a UTF-8 string into a list of Unicode codepoints.
     * @param str UTF-8 encoded string.
     * @return Vector of decoded codepoints.
     */
    std::vector<uint32_t> utf8_to_codepoints(const std::string& str);

    /**
     * @brief Singleton that loads fonts and renders text via FreeType.
     *
     * Rasterizes glyphs on demand, caches them per @ref Font, and draws text
     * as textured quads.
     */
    class TextRenderer {
        private:
        static TextRenderer* instance; ///< Singleton instance.
        FT_Library ft;                 ///< FreeType library handle.
        /**
         * @brief Initializes the FreeType library.
         * @return @c true on success, @c false on failure.
         */
        bool init_freetype();
        std::unordered_map<std::string, std::unique_ptr<Font>> fonts; ///< Loaded fonts keyed by path.

        public:
        bool valid; ///< Whether the renderer initialized successfully.
        /**
         * @brief Clears the cached glyphs for a font.
         * @param font Font whose glyph cache should be emptied.
         */
        void clearGlyphCache(Font* font);
        /**
         * @brief Loads a font from disk (or returns the cached one).
         * @param path Path to the font file.
         * @param height Pixel height to render the font at.
         * @return Pointer to the loaded font, or @c nullptr on failure.
         */
        Font* loadFont(std::string path, int height = 40);
        /**
         * @brief Changes a font's pixel height and clears its glyph cache.
         * @param font Font to modify.
         * @param heightIn New pixel height.
         */
        void setFontHeight(Font* font, int heightIn);
        /**
         * @brief Rasterizes and caches a single glyph.
         * @param font Font to load the glyph for.
         * @param cp Unicode codepoint of the glyph.
         * @return @c true if the glyph was loaded successfully.
         */
        bool loadGlyph(Font* font, uint32_t cp);
        /**
         * @brief Draws a text string at a position.
         * @param font Font to render with.
         * @param text UTF-8 text to draw.
         * @param pos Baseline position (z is the layer).
         * @param scale Scale factor applied to the glyphs.
         * @param color Text color.
         */
        void renderText(Font* font, const std::string &text, Vector3 pos, float scale, const Color &color);
        /**
         * @brief Measures the rendered size of a text string.
         * @param font Font that would render the text.
         * @param text UTF-8 text to measure.
         * @param scale Scale factor applied to the glyphs.
         * @return The width and height the text would occupy.
         */
        Vector2 measureText(Font *font, const std::string &text, float scale);
        /**
         * @brief Returns the singleton instance.
         * @return Pointer to the TextRenderer instance.
         */
        static TextRenderer* getInstance();
        /// @brief Constructs the renderer and initializes FreeType.
        TextRenderer();
        /// @brief Releases FreeType resources.
        ~TextRenderer();

    };
}