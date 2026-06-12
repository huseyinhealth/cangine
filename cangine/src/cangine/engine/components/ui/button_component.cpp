#include "button_component.hpp"

#include "../../gameobjects/ui/button_object.hpp"

namespace cangine {
    void ButtonComponent::firstFrame() {
        this->button = (Button*) this->getGameObject();
        this->trc = this->button->getTextRendererComponent();
    }

    void ButtonComponent::onMouseEvent(const MouseEvent *e) {
        Rectangle& rect = this->button->getTextRendererComponent()->rect;
        this->mouseHover = (e->pos.x >= rect.pos.x - (rect.width / 2.0f) && e->pos.x <= rect.pos.x + (rect.width / 2.0f)) &&
                        (e->pos.y >= rect.pos.y - (rect.height / 2.0f) && e->pos.y <= rect.pos.y + (rect.height / 2.0f));

        if (e->event == MouseEventType::MouseClick && this->mouseHover) {
            this->mousePressed = true;
            this->button->setBackgroundColor(this->focusColor);
        } else if (e->event == MouseEventType::MouseRelease) {
            if (this->mousePressed && this->mouseHover) {
                this->button->onClick(e);
            }

            this->mousePressed = false;
            this->button->setBackgroundColor(this->mouseHover ? this->hoverColor : this->backgroundColor);
        } else if (e->event == MouseEventType::CursorMove) {
            if (!this->mousePressed) {
                this->button->setBackgroundColor(this->mouseHover ? this->hoverColor : this->backgroundColor);
            }
        }
    }
    void ButtonComponent::setTexture(Texture *textureIn) {
        this->trc->setBackTexture(textureIn);
    }
    Texture *ButtonComponent::getTexture() {
        return this->trc->getBackTexture();
    }

    
}
