#include "panel_component.hpp"

#include <cassert>

namespace cangine {
    void PanelComponent::registerChild(GameObject *childIn) {
        this->registerChild(new PanelChild {
            childIn,
            Transform::posFrom(childIn)
        });
    }

    void PanelComponent::registerChild(PanelChild* childIn) {
        for (auto child : this->children) {
            assert(child->go != childIn->go && "Can not add child twice to panel.");
        }

        this->children.push_back(childIn);        
        Vector3 newPos = this->getWorldPosition(childIn->go);
        newPos.z = Transform::posFrom(this->getGameObject()).z + 1;
        Transform::posFor(childIn->go, newPos);
    }

    void PanelComponent::firstFrame() {
        assert(RenderSystem::getInstance() != nullptr);

        this->qr = RenderSystem::getInstance()->quadRenderer;
        this->texture = nullptr;

        this->transform = this->getGameObject()->getComponent<Transform>();
        this->pos = Vector3(0, 0, 80);
    }

    void PanelComponent::frame(double _dt) {
        this->rect = {
            transform->getPosition(),
            this->width + this->padding * 2,
            this->height + this->padding * 2,
            this->color
        };

        Vector3 objPos = this->transform->getPosition();
        Vector3 rectPos(
            objPos.x - this->padding / 2,
            objPos.y - this->padding / 2,
            objPos.z
        );

        this->rect.pos = rectPos;
        
        Transform rectTr;
        rectTr.setPosition(rectPos);
        rectTr.setRotation(this->transform->getRotation());
        rectTr.setScale(this->transform->getScale());
        
        this->qr->setTexture(this->texture);
        this->qr->draw(rectTr, this->rect);

        Vector3 panelPos = this->transform->getPosition();

        if ((
            panelPos.x != pos.x ||
            panelPos.y != pos.y ||
            panelPos.z != pos.z
        )){
            for (auto child : children) {
                Vector3 childPos = Transform::posFrom(child->go);
                Vector3 newPos = this->getWorldPosition(child->go);
                Transform::posFor(child->go, newPos);
            }

            this->pos = panelPos;
        }
    }
    
    Vector3 PanelComponent::getWorldPosition(GameObject* gameObject) {
        Vector2 v1;
        Vector2 v2 = Vector2(this->transform->getPosition().x, this->transform->getPosition().y);
        PanelChild* obj = this->getPanelObject(gameObject);
        if (obj == nullptr) return Vector3();
        v1 = Vector2(obj->offset.x, obj->offset.y);
        return Vector3(v1 + v2, this->pos.z + 1);
    }

    Vector3 PanelComponent::getOffset(GameObject* go) {
        return this->getPanelObject(go)->offset;
    }

    PanelChild* PanelComponent::getPanelObject(GameObject* gameObject) {
        for (auto obj : this->children) {
            if (obj->go == gameObject)
                return obj;
        }

        return nullptr;
    }

        void PanelComponent::setRectColor(Color colorIn) {
        this->color = colorIn;
    }

    void PanelComponent::setRectWidth(int widthIn) {
        this->width = widthIn;
    }

    void PanelComponent::setRectHeight(int heightIn) {
        this->height = heightIn;
    }

    void PanelComponent::setOffset(GameObject* gameobject, Vector3 offsetIn) {
        for (auto c : this->children) {
            if (c->go == gameobject) {
                this->setOffset(c, offsetIn);
                return;
            }
        }
        std::cout << "PanelComponent: setOffset: Given object is not " <<
        "a child of this panel. Object address: " << gameobject << "\n";
    }

    void PanelComponent::setOffset(PanelChild* panelChild, Vector3 offsetIn) {
        panelChild->offset = offsetIn;
        Transform::posFor(panelChild->go, this->getWorldPosition(panelChild->go));
    }

    PanelComponent::~PanelComponent() {
        for (auto c : this->children) {
            delete c;
        }
    }
}
