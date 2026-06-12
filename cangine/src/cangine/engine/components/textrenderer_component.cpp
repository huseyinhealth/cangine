#include "textrenderer_component.hpp"
#include "../../render/renderer/render_system.hpp"
#include "../../render/renderer/quad_renderer.hpp"
#include "../gameobjects/game_object.hpp"
#include "transform_component.hpp"

#ifndef _WIN32
#include <fontconfig/fontconfig.h>
#endif

namespace cangine {
    void TextRendererComponent::firstFrame() {
        this->tr = RenderSystem::getInstance()->textRenderer;
        this->color = Colors::WHITE;
        this->backgroundColor = Color(0, 0, 0, 0);  // transparent
        this->backgroundPadding = 5.0f; // 5 pixel default padding
        this->fontHeight = 20;
        this->scale = 1;
        
        #ifdef _WIN32
        this->path = "C:\\Windows\\Fonts\\arial.ttf";
        #else
        {
            FcConfig* fc = FcInitLoadConfigAndFonts();
            FcPattern* pat = FcNameParse((const FcChar8*)"sans-serif");
            FcConfigSubstitute(fc, pat, FcMatchPattern);
            FcDefaultSubstitute(pat);
            FcResult res;
            FcPattern* match = FcFontMatch(fc, pat, &res);
            if (match) {
                FcChar8* file = nullptr;
                if (FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch)
                    this->path = reinterpret_cast<char*>(file);
                FcPatternDestroy(match);
            }
            FcPatternDestroy(pat);
            FcConfigDestroy(fc);
        }
        #endif

        this->text = "Hello, world!";

        this->font = this->tr->loadFont(this->path, this->fontHeight);
        
        if (!this->font || !this->font->face) {
            std::cerr << "TextRendererComponent: Unable to load font: " << this->path << "\n";
            return;
        }
        

        #ifdef CANGINE_DEBUG
            std::cout << "Font loaded: " << (this->font != nullptr) << "\n";
            for (auto cp : utf8_to_codepoints(this->text)) {
                std::cout << "Glyph for " << (char)cp << ": " << this->font->glyphs.count(cp) << "\n";
            }
        #endif
    }

    void TextRendererComponent::frame(double _dt) {
        Vector2 textSize = this->getSize();
        
        Transform* t = this->getGameObject()->getComponent<Transform> ();
        Vector3 pos = t->getPosition();

        QuadRenderer* qr = RenderSystem::getInstance()->quadRenderer; 
        Vector3 rectPos = Vector3(
            pos.x + (this->getSize().x / 2),
            pos.y + (this->getSize().y / 2),
            t->getPosition().z
        );

        if (this->backgroundColor.a != 0) {
            this->rect = {
                rectPos,
                textSize.x + (this->backgroundPadding * 2),
                textSize.y + (this->backgroundPadding * 2),
                this->backgroundColor
            };
            
            Transform rectTr;
            rectTr.setPosition(rectPos);
            rectTr.setRotation(t->getRotation());
            rectTr.setScale(t->getScale());

            qr->setTexture(this->backTex);
            qr->draw(rectTr, rect);
        }
        
        this->tr->renderText(this->font, this->text, t->getPosition(), this->scale, this->color);
    }

    Vector2 TextRendererComponent::getSize() {
        return this->tr->measureText(this->font, this->text, this->scale);
    }
    void TextRendererComponent::setFontHeight(int heightIn) {
        this->font = this->tr->loadFont(this->path, heightIn);
        this->fontHeight = heightIn;
    }
    int TextRendererComponent::getFontHeight() {
        return this->fontHeight;
    }
    void TextRendererComponent::setBackTexture(Texture* textureIn) {
        // if (this->backTex != nullptr) delete this->backTex;
        this->backTex = textureIn;
    }
    Texture* TextRendererComponent::getBackTexture() const {
        return this->backTex;
    }
}
