#include "label_object.hpp"

namespace cangine {
    Label::Label(std::string text, Vector3 position) : GameObject() {
        this->trc = this->newComponent<TextRendererComponent> ();
        this->trc->text = text;
        Transform::posFor(this, position);
    }

    std::string Label::getText() {
        return this->trc->text;
    }

    Vector2 Label::getSize() {
        return this->trc->getSize();
    }

    int Label::getFontHeight() {
        return this->trc->getFontHeight();
    }

    void Label::setText(std::string text) {
        this->trc->text = text;
    }

    void Label::setFontHeight(int heightIn) {
        this->trc->setFontHeight(heightIn);
    }

    void Label::setColor(Color colorIn) {
        this->trc->color = colorIn;
    }

    void Label::setBackgroundColor(Color colorIn) {
        this->trc->backgroundColor = colorIn;
    }

    TextRendererComponent* Label::getTextRendererComponent() {
        return this->trc;
    }
}
