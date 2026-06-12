#include "text_renderer.hpp"

#include <cassert>

#include "render_system.hpp"

namespace cangine {

    std::vector<uint32_t> utf8_to_codepoints(const std::string& str) {
        std::vector<uint32_t> result;
        size_t i = 0;
        while (i < str.size()) {
            uint32_t cp = 0;
            unsigned char c = str[i];
            if (c <= 0x7F) {
                cp = c;
                i += 1;
            } else if ((c & 0xE0) == 0xC0) {
                cp = ((str[i] & 0x1F) << 6) | (str[i+1] & 0x3F);
                i += 2;
            } else if ((c & 0xF0) == 0xE0) {
                cp = ((str[i] & 0x0F) << 12) | ((str[i+1] & 0x3F) << 6) | (str[i+2] & 0x3F);
                i += 3;
            } else if ((c & 0xF8) == 0xF0) {
                cp = ((str[i] & 0x07) << 18) | ((str[i+1] & 0x3F) << 12) | ((str[i+2] & 0x3F) << 6) | (str[i+3] & 0x3F);
                i += 4;
            } else {
                i += 1; // invalid UTF-8
            }
            result.push_back(cp);
        }
        return result;
    }

    TextRenderer* TextRenderer::instance = nullptr;

    void TextRenderer::clearGlyphCache(Font* font) {
        assert(font != nullptr);
        font->glyphs.clear();
    }

    bool TextRenderer::init_freetype() {
            if (FT_Init_FreeType(&this->ft)) {
                std::cerr << "Failed to init FreeType!" << std::endl;
                return 0;
            }
            std::cout << "FreeType initialized successfully!" << std::endl;
            return 1;
        }

        Font* TextRenderer::loadFont(std::string path, int height) {
            for (auto& f : fonts) {
                if (f.second->path == path && f.second->height == height)
                        return f.second.get();
            }

            FT_Face face;
            if (FT_New_Face(ft, path.c_str(), 0, &face)) {
                std::cerr << "Failed to load font: " << path << "\n";
                return nullptr;
            }

            FT_Set_Pixel_Sizes(face, 0, height);

            auto font = std::make_unique<Font>(face, height, path);
            
            std::string defaultCharset = 
                "abcdefghijklmnopqrstuvwxyz"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "0123456789"
                "çğıöşüÇĞİÖŞÜ"
                "!@#$%^&*()-_=+[]{};:'\",.<>/?|\\`~ ";

            for (uint32_t cp : utf8_to_codepoints(defaultCharset))
                loadGlyph(font.get(), cp);

            Font* fontPtr = font.get();
            fonts[path + std::to_string(height)] = std::move(font);

            return fontPtr;
        }

        void TextRenderer::setFontHeight(Font* font, int heightIn) {
            assert(font != nullptr && "Font pointer is null");
            FT_Error error = FT_Set_Pixel_Sizes(font->face, 0, heightIn);
            if (error) {
                std::cerr << "FreeType Error (" << error << "): Failed to set pixel size to " << heightIn << std::endl;
                return;
            }
            font->height = heightIn;
        }

        bool TextRenderer::loadGlyph(Font* font, uint32_t cp) {
            if (!font || !font->face) return false;
            if (font->glyphs.find(cp) != font->glyphs.end())
                return true;


            if (FT_Get_Char_Index(font->face, cp) == 0) {
                cp = '?'; // fallback
                if (FT_Get_Char_Index(font->face, cp) == 0)
                    return false;
            }

            if (FT_Load_Char(font->face, cp, FT_LOAD_RENDER))
                return false;

            FT_GlyphSlot g = font->face->glyph;

            Character ch;
            int w = g->bitmap.width;
            int h = g->bitmap.rows;

            std::vector<unsigned char> tempBuffer(w * h);
            for (int row = 0; row < h; ++row)
                memcpy(&tempBuffer[row * w], g->bitmap.buffer + (h - 1 - row) * w, w);

            ch.texture.createFromBuffer(w, h, tempBuffer.data(), true);
            ch.width = w;
            ch.height = h;
            ch.bearingX = g->bitmap_left;
            ch.bearingY = g->bitmap_top;
            ch.advance = g->advance.x;

            font->glyphs[cp] = std::move(ch);
            return true;
        }

        void TextRenderer::renderText(Font* font, const std::string& text, Vector3 pos, float scale, const Color& color) {
            auto codepoints = utf8_to_codepoints(text);
            QuadRenderer* qr = RenderSystem::getInstance()->quadRenderer;

            for (uint32_t cp : codepoints) {
                if (!loadGlyph(font, cp)) continue;

                Character& ch = font->glyphs[cp];
                float xpos = pos.x + ch.bearingX * scale;
                float ypos = pos.y - (ch.height - ch.bearingY) * scale;
                float w = ch.width * scale;
                float h = ch.height * scale;

                Rectangle r;
                r.pos = {xpos, ypos, pos.z};
                r.width = w;
                r.height = h;
                r.color = color;

                qr->setTexture(&ch.texture, true);
                qr->draw(r);
                
                #ifdef CANGINE_DEBUG

                    std::cout << "DRAW: " << (char)cp
                        << " x=" << xpos
                        << " y=" << ypos << "\n";

                #endif

                // std::cout << "Draw color: " <<
                // color.r << " " <<
                // color.g << " " <<
                // color.b << " " <<
                // color.a << "\n";

                pos.x += (ch.advance >> 6) * scale;
            }
        }

        Vector2 TextRenderer::measureText(Font* font, const std::string& text, float scale = 1.0f) {
            auto codepoints = utf8_to_codepoints(text);

            float width = 0.0f;
            float maxAbove = 0.0f; // max bearingY
            float maxBelow = 0.0f; // max (height - bearingY)

            for (uint32_t cp : codepoints) {
                if (!loadGlyph(font, cp))
                    continue;

                Character& ch = font->glyphs[cp];

                width += (ch.advance >> 6) * scale;

                float above = ch.bearingY * scale;
                float below = (ch.height - ch.bearingY) * scale;

                if (above > maxAbove) maxAbove = above;
                if (below > maxBelow) maxBelow = below;
            }

            Vector2 size;
            size.x  = width;
            size.y = maxAbove + maxBelow;
            return size;
        }

        TextRenderer* TextRenderer::getInstance() {
            return instance;
        }

        TextRenderer::TextRenderer() {
            assert(instance == nullptr && "Access text renderer from `RenderSystem::textRenderer`!");
            this->valid = this->init_freetype();
            assert(this->valid);
            
            // std::string defaultCharset = 
            // "abcdefghijklmnopqrstuvwxyz"
            // "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            // "0123456789"
            // "çğıöşüÇĞİÖŞÜ"
            // "!@#$%^&*()-_=+[]{};:'\",.<>/?|\\`~ ";

            // for (uint32_t cp : utf8_to_codepoints(defaultCharset))
            //     this->loadGlyph(cp);

            instance = this;
        }

        TextRenderer::~TextRenderer(){
            for (auto& f : fonts) {
                FT_Done_Face(f.second->face);
            }
            FT_Done_FreeType(this->ft);
            fonts.clear();
        }
}