
#include "game_object.hpp"
#include "../object_manager.hpp"
#include "../components/transform_component.hpp"
#include "../components/quadrenderer_component.hpp"

namespace cangine {
    GameObject::GameObject() {
        ObjectManager *objMan = ObjectManager::getInstance();
        assert(objMan != nullptr);
        objMan->registerObject(this);
        Transform *t = new Transform();
        this->appendComponent<Transform>(t);

        t->setPosition(Vector3(0 , 0, 0));
    }

    void GameObject::onMouseEvent(const MouseEvent* e) {
        for (auto *component : this->components) {
            component->onMouseEvent(e);
        }
    }

    void GameObject::setActive(bool a) {
        this->active = a;
        if (a) this->onEnable();
        else this->onDisable();
    }

    bool GameObject::isActive() {
        return this->active;
    }

    GameObject::~GameObject() {
        for (auto component : components) {
            delete component;
        }
    }

    void GameObject::onUpdate(double deltaTime) {
        for (auto *component : this->components) {
            component->frame(deltaTime);
        }
    }
}
