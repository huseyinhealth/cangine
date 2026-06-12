#include "object_manager.hpp"
#include "./gameobjects/game_object.hpp"
#include "../platform/window.hpp"

#include <algorithm>

namespace cangine {
    ObjectManager* ObjectManager::instance = nullptr;

    ObjectManager::ObjectManager() {
        win = Window::getInstance();
        assert(win != nullptr);
        assert(instance == nullptr);
        instance = this;
    }

    ObjectManager::~ObjectManager() {
        for (auto o : this->objects) {
            delete o;
        }
        this->instance = nullptr;
    }

    ObjectManager* ObjectManager::getInstance() {
        return instance;
    }

    void ObjectManager::registerObject(GameObject* obj) {
        this->objects.push_back(obj);
    }

    void ObjectManager::updateAll() {
        // painter's algorithm... i guess?
        std::sort(this->objects.begin(), this->objects.end(), [](GameObject* a, GameObject* b) {
            return Transform::posFrom(a).z < Transform::posFrom(b).z;
        });

        for (auto *object : this->objects) {
            if (object->isActive())
                object->onUpdate(win->getDeltaTime());
        }

        auto it = std::remove_if(this->objects.begin(), this->objects.end(),
            [](GameObject* obj) {
                if (obj->pendingDestroy) {
                    delete obj;
                    return true;
                }
                return false;
            }
        );
        this->objects.erase(it, this->objects.end());
    }

    void ObjectManager::fireMouseEvent(const MouseEvent* event) {
        for (auto *object : this->objects) {
            object->onMouseEvent(event);
        }
    }
}
