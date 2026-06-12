#include "sandbox.hpp"

#include <cangine/audio/sound.hpp>
#include <cangine/audio/audiomanager.hpp>

/**
 * 
 * This program is used for testing the engine.
 * 
 * This code is pretty messy and its not recommended
 * to use the engine this way.
 * 
 * Check `simplehelloworld.cpp` for more clean usage
 * of the engine.
 * 
 * - huseyinhealth
 * 
 */

cangine::Color background(51, 102, 102);

class TestComponent : public cangine::IComponent{
    private:
    cangine::QuadRendererComponent* qrc;
    bool wasColliding = false;

    public:
    cangine::Rectangle* otherRect;
    cangine::Transform* otherTransform;
    float* rotSpeed;

    void firstFrame() override {
        cangine::GameObject* gameObject = this->getGameObject();

        this->qrc = new cangine::QuadRendererComponent();
        gameObject->appendComponent<cangine::QuadRendererComponent>(this->qrc);

        this->qrc->setRectWidth(128);
        this->qrc->setRectHeight(64);
        this->qrc->setRectColor(cangine::Colors::WHITE);
    }

    void frame(double dt) override {
        cangine::Transform* myTransform = this->getGameObject()->getComponent<cangine::Transform>();

        bool isColliding = cangine::Rectangle::overlaps(
            *this->otherRect,      *this->otherTransform,
            *this->qrc->getRect(), *myTransform
        );

        if (isColliding && !wasColliding) {
            *rotSpeed = -*rotSpeed;
        }

        wasColliding = isColliding;
    }

    void onMouseEvent(const cangine::MouseEvent* e) override {
        if (e->event == cangine::MouseEventType::MouseClick) {
            cangine::Transform* myTransform = this->getGameObject()->getComponent<cangine::Transform>();

            if (cangine::Rectangle::containsPoint(*this->qrc->getRect(), *myTransform, e->pos)) {
                std::cout << "Clicked to white rect!\n";
                std::cout << this->otherRect->pos.x << " " << this->otherRect->pos.y << " "
                          << this->otherRect->width << " " << this->otherRect->height << "\n";

                if (cangine::Rectangle::overlaps(
                    *this->otherRect,      *this->otherTransform,
                    *this->qrc->getRect(), *myTransform)) {
                    std::cout << "Colliding!\n";
                } else {
                    std::cout << "Not Colliding!\n";
                }
            }
        }
    }
};

int run()
{
    using namespace cangine;
    init();

    {
        Window window(800, 600, "cangine");
        Shader defaultShader("default");
        if (!defaultShader.valid) {
            std::cout << "Invalid shader program\n";
            return -2;
        }

        auto renderer = std::make_unique<RenderSystem>(defaultShader);
        auto objMan   = std::make_unique<ObjectManager>();
        
        Texture t("cangine.png");

        auto* test       = new GameObject();
        auto* rendercomp = new QuadRendererComponent();
        test->appendComponent<QuadRendererComponent>(rendercomp);
        rendercomp->setRectWidth(128);
        rendercomp->setRectHeight(128);
        rendercomp->setRectColor(Colors::WHITE);

        Transform* tT = test->getComponent<Transform>();
        tT->setPosition(Vector3(0, 0, 1));
        rendercomp->setTexture(t);

        auto* textObj = new GameObject();
        auto* trc     = new TextRendererComponent();
        textObj->appendComponent<TextRendererComponent>(trc);
        trc->color = Colors::GREEN;
        trc->backgroundColor = Colors::BLACK;
        trc->backgroundColor.a = 0.5f;

        auto* mouesEvent = new GameObject();
        auto* met        = new TestComponent();
        met->otherRect = rendercomp->getRect();
        met->otherTransform = test->getComponent<Transform>();
        mouesEvent->appendComponent<TestComponent>(met);
        mouesEvent->getComponent<Transform>()->setPosition(Vector3(0, 100, 1));

        float newRotf = 0;
        float rotSpeed = 0.5f;
        met->rotSpeed = &rotSpeed;

        while (!window.shouldClose()) {
            window.fill(background);
            window.beginFrame();

            std::string fpsStr = "FPS: " + std::to_string(window.getInstantFPS());
            textObj->getComponent<Transform>()->setPosition(
                Vector3(
                    window.dimensions.width  / -2 + trc->backgroundPadding,
                    window.dimensions.height /  2 - trc->getSize().y - trc->backgroundPadding,
                    80
                )
            );
            trc->text = fpsStr;

            if (Input::getKey(Key::A))
                std::cout << "GetKey A\n";

            if (Input::getKeyDown(Key::B)) {
                std::cout << "GetKeyDown B\n";
                Transform* testTr = test->getComponent<Transform>();
                Vector3 testPos = testTr->getPosition();
                testPos.y -= 5;
                testTr->setPosition(testPos);
            }

            if (Input::getKeyUp(Key::C))
                std::cout << "GetKeyUp C\n";

            test->getComponent<Transform>()->setRotation({ 0, 0, newRotf });
            newRotf += rotSpeed;

            window.endFrame();
        }

    }

    terminate();
    std::cout << "Successfully exit the program.\n";
    return 0;
}

int main() {
    return run();
}
