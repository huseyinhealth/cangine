#include "button_object.hpp"

#include "../../components/ui/button_component.hpp"

namespace cangine {
    void Button::onClick(const MouseEvent* e)
    {
        for (std::function<void(const MouseEvent*)> handler: this->onClickHandlers)
            handler(e);
    }

    Button::Button(
        std::string text,
        Vector3 position,
        Color foreColor,
        Color hoverColor,
        Color focusColor,
        Color backgroundColor
    ) : Label(text, position) {
        this->setText(text);
        this->setColor(foreColor);
        this->setBackgroundColor(backgroundColor);

        this->btnComponent = this->newComponent<ButtonComponent> ();
        this->btnComponent->hoverColor = hoverColor;
        this->btnComponent->focusColor = focusColor;
        this->btnComponent->backgroundColor = backgroundColor;

        Transform::posFor(this, Vector3(0, 0, 80));
    }
    void Button::setTexture(Texture *textureIn) {
        this->btnComponent->setTexture(textureIn);
    }

    Texture* Button::getTexture() {
        return this->btnComponent->getTexture();
    }
}
