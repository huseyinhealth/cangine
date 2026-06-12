#include "quadrenderer_component.hpp"
#include <cassert>


namespace cangine {
    void QuadRendererComponent::firstFrame() {
        assert(RenderSystem::getInstance() != nullptr);

        this->qr = RenderSystem::getInstance()->quadRenderer;
        this->texture = nullptr;

        this->transform = this->getGameObject()->getComponent<Transform>();
        this->rect = {transform->getPosition(), 50, 50, Colors::WHITE};
    }

    void QuadRendererComponent::frame(double _dt) {
        this->qr->setTexture(texture);
        this->rect.pos = this->getGameObject()->getComponent<Transform> ()->getPosition();
        this->qr->draw(*this->transform, this->rect);
    }

    void QuadRendererComponent::setTexture(Texture &t) {
        this->texture = &t;
    }

    void QuadRendererComponent::setRectWidth(int widthIn) {
        this->rect.width = widthIn;
    }
    void QuadRendererComponent::setRectHeight(int heightIn) {
        this->rect.height = heightIn;
    }
    void QuadRendererComponent::setRectColor(Color colorIn) {
        this->rect.color = colorIn;
    }

    Rectangle* QuadRendererComponent::getRect() {
        return &rect;
    }

    Vector3 QuadRendererComponent::getRectPos() {
        return this->rect.pos;
    }

    Vector2 QuadRendererComponent::getRectSize() {
        return Vector2(this->rect.width, this->rect.height);
    }
}
