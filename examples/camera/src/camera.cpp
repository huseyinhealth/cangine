#include "camera.hpp"

using namespace cangine;

class CustomCamera : public IComponent {
    private:
    bool dragging = false;
    Vector2 lastPos = {0, 0};
    Vector2 currentPos {0, 0};
    public:
    void onMouseEvent(const MouseEvent* e) override {
        if (e->button == MouseButton::Right) {
            if (e->event == MouseEventType::MouseClick) {
                this->dragging = true;
                this->lastPos = e->pos;
            } else if (e->event == MouseEventType::MouseRelease) {
                this->dragging = false;
            }
        }

        if (e->event == MouseEventType::CursorMove && this->dragging) {
            Vector2 delta = e->pos - this->lastPos;
            // std::cout << "delta: " << delta.x << " " << delta.y << "\n";
            this->lastPos = e->pos;

            Transform* t = this->getGameObject()->getComponent<Transform>();
            Vector3 pos = t->getPosition();
            t->setPosition(pos + Vector3(delta, pos.z));
        }
    }
};

class CameraExample : public CangineApp {
    public:
    CameraExample() : CangineApp(800, 600, "cangine :: Camera Example") {}

    void onFirstFrame() override {
        Camera* camera = new Camera();  // camera component gets added automatically
        CustomCamera* cc = new CustomCamera();
        camera->appendComponent<CustomCamera> (cc);

        GameObject* rect = new GameObject();
        QuadRendererComponent* qrc1 = new QuadRendererComponent();
        rect->appendComponent<QuadRendererComponent> (qrc1);
        
        GameObject* text = new GameObject();
        TextRendererComponent* trc = new TextRendererComponent();
        text->appendComponent<TextRendererComponent> (trc);
        trc->text = "Right click to drag camera. (drag to left for surprise)";
        text->getComponent<Transform>()->setPosition(Vector3(trc->getSize().x / -2, 100, 1));

        GameObject* text2 = new GameObject();
        TextRendererComponent* trc2 = new TextRendererComponent();
        text2->appendComponent<TextRendererComponent> (trc2);
        trc2->text = "You did it!";
        text2->getComponent<Transform>()->setPosition(Vector3(800, 100, 1));
        
        GameObject* text3 = new GameObject();
        TextRendererComponent* trc3 = new TextRendererComponent();
        text3->appendComponent<TextRendererComponent> (trc3);
        trc3->text = "This text is located in the UI layer. (Z index above 80)";
        trc3->backgroundColor = Colors::BLUE;
        text3->getComponent<Transform>()->setPosition(Vector3(trc3->getSize().x / -2, -100, 81));
    } 
};

int main() {
    CameraExample().run();
    return 0;
}
