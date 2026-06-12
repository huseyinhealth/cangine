#include "panel_object.hpp"

namespace cangine {
    Panel::Panel(int padding) : GameObject() {
        this->pc = this->newComponent<PanelComponent> ();
        this->setPadding(padding);
        Transform::posFor(this, Vector3(0, 0, 80));
    }

    void Panel::setPadding(int paddingIn) {
        this->pc->padding = paddingIn;
    }

    void Panel::registerChild(GameObject* childIn) {
        this->pc->registerChild(childIn);
    }

    void Panel::setRectColor(Color colorIn) {
        this->pc->setRectColor(colorIn);
    }
    
    void Panel::setRectWidth(int widthIn) {
        this->pc->setRectWidth(widthIn);
    }

    void Panel::setRectHeight(int heightIn) {
        this->pc->setRectHeight(heightIn);
    }

    void Panel::setOffset(GameObject* go, Vector3 offset) {
        this->pc->setOffset(go, offset);
    }

    Vector3 Panel::getOffset(GameObject* go) {
        return this->pc->getOffset(go);
    }

    Vector2 Panel::getRectSize() {
        return Vector2(this->pc->getRect()->width, this->pc->getRect()->height);
    }

    void Panel::onDisable() {
        for (auto child : this->pc->children) {
            if (!child->go->isActive()) {
                this->notActive.insert(child);
            }

            child->go->setActive(false);
        }
    }

    void Panel::onEnable() {
        for (auto child : this->pc->children) {
            bool cactive = true;
            for (auto na : this->notActive) {
                if (na == child) {
                    cactive = false;
                    break;
                }
            }

            if (cactive) child->go->setActive(true);
        }

        this->notActive.clear();
    }
}
