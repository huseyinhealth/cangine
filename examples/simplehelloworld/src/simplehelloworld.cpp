#include "simplehelloworld.hpp"

class HelloWorldApp : public cangine::CangineApp {
    public:
    HelloWorldApp() : CangineApp(800, 600, "cangine :: Hello World") {}
    
    void onFirstFrame() override {
        using namespace cangine;
        // This method is only called once.

        // init gameobject
        Label* helloWorldText = this->instantiateObject<Label> ();
        helloWorldText->setText("Hello, world!");

        // Center the text                         get text size (Vector2)
        Transform::posFor(helloWorldText, Vector3(helloWorldText->getSize() / -2, 80));
    }
};

int main() {
    HelloWorldApp().run();
    return 0;
}
