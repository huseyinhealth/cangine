#include "position.hpp"
#include "../platform/window.hpp"

#include "../engine/components/camera_component.hpp"
#include "../engine/components/transform_component.hpp"
#include "../engine/gameobjects/game_object.hpp"

namespace cangine {
    Window* Position::window_instance = nullptr;

    Vector2 Position::normalize(const Vector2& p) {
        float halfW = window_instance->dimensions.width * 0.5f;
        float halfH = window_instance->dimensions.height * 0.5f;

        Vector2 normalized = {
            p.x / halfW,
            p.y / halfH
        };

        CameraComponent* cameraInstance = CameraComponent::getInstance();
        if (cameraInstance == nullptr)
            return normalized;
        Transform* cameraTransform = cameraInstance->getGameObject()->getComponent<Transform> ();
        Vector3 cameraPos = cameraTransform->getPosition();           
        return normalized + Vector2(cameraPos.x / halfW, cameraPos.y / halfH);

    }

    Vector3 Position::normalize(const Vector3& p) {
        float halfW = window_instance->dimensions.width * 0.5f;
        float halfH = window_instance->dimensions.height * 0.5f;
        Vector3 normalized = {
            p.x / halfW,
            p.y / halfH,
            1.0f - (2.0f * p.z / POSITION_MAX_LAYER)
        };

        CameraComponent* cameraInstance = CameraComponent::getInstance();
        if (cameraInstance == nullptr || p.z >= 80)  // no camera or ui layer
            return normalized;

        Transform* cameraTransform = cameraInstance->getGameObject()->getComponent<Transform> ();
        Vector3 cameraPos = cameraTransform->getPosition();           
        // std::cout << "camPos: " << cameraPos.x << " " << cameraPos.y << "\n";
        return normalized + Vector2(cameraPos.x / halfW, cameraPos.y / halfH);
    }

    Vertex Position::normalize(const Vertex& v) {
        float halfW = window_instance->dimensions.width * 0.5f;
        float halfH = window_instance->dimensions.height * 0.5f;

        Vertex normalized {
            v.x / halfW,
            v.y / halfH,
            1.0f - (2.0f * v.z / POSITION_MAX_LAYER),
            v.color
        };
        CameraComponent* cameraInstance = CameraComponent::getInstance();
        if (cameraInstance == nullptr || v.z >= 80)
            return normalized;

        Transform* cameraTransform = cameraInstance->getGameObject()->getComponent<Transform>();
        Vector3 cameraPos = cameraTransform->getPosition();
        normalized.x += cameraPos.x / halfW;
        normalized.y += cameraPos.y / halfH;
        return normalized;
    }

}


