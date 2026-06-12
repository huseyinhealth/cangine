#include "ui.hpp"

using namespace cangine;

class UIExample : public CangineApp {
    public:
    UIExample() : CangineApp(800, 600, "cangine :: UI Example") {}
    static int counter;
    Label* lbl = nullptr;

    void onFirstFrame() override {
        counter = 0;
        Button* btn = new Button("Click me!");
        Transform::posFor((GameObject*) btn,  Vector3(btn->getSize().x / -2, 0, 80));  // Z indez 80 = ui layer
        this->lbl = new Label("This is a counter. Click button to increase!");
        Transform::posFor((GameObject*) lbl,  Vector3(lbl->getSize().x / -2, 30, 80));

        // add click event handler
        btn->registerOnClickEvent([&](const MouseEvent* e) {
            counter++;
        });
    }

    void onFrame(double _dt) override {
        if (!counter == 0) {
            this->lbl->setText(std::to_string(counter));
            Transform::posFor((GameObject*) this->lbl, 
                Vector3(this->lbl->getSize().x / -2, 30, 80));
        }
    }

};
int UIExample::counter = 0;

int main() {
    UIExample().run();
    return 0;
}
