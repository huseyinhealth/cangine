#include "panel.hpp"

using namespace cangine;

class PanelExample : public cangine::CangineApp {
    public:
    Panel* panel;
    int direction = 1;
    PanelExample() : CangineApp(800, 600, "cangine :: Panel Example") {};

    void onFirstFrame() override {
        // Init panel
        this->panel = this->instantiateObject<Panel>();

        // Init button and register button to panel
        Button* exitButton = this->instantiateObject<Button>("Exit");
        this->panel->registerChild(exitButton);

        exitButton->registerOnClickEvent(
            [](const MouseEvent* _e) {
                exit(0);
            }
        );
    }

    void onFrame(double dt) override {
        // Move panel and make it bounce from left and right sides
        // Not touching button.

        Vector3 panelpos = Transform::posFrom(this->panel);

        Vector2 rs = this->panel->getRectSize();
        Dimensions d = Window::getInstance()->dimensions;

        int halfWidth = d.width / 2;

        if (panelpos.x + (rs.x / 2) >= halfWidth) {
            direction = -1;
        } else if (panelpos.x - (rs.x / 2) <= -halfWidth) {
            direction = 1;
        }

        panelpos.x += direction * 100 * dt;
        Transform::posFor(this->panel, panelpos);
    }
};

int main() {
    PanelExample().run();
    return 0;
}
