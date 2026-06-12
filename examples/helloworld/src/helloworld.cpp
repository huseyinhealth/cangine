#include "helloworld.hpp"

int main() {
    using namespace cangine;

    // init

    init();

    {
        // init window, shaders, etc.
        Window win(800, 600, "Cangine");
        Shader defaultShader("default");
        auto _rs = std::make_unique<RenderSystem>(defaultShader);
        auto objMan   = std::make_unique<ObjectManager>();

        // Create Game Object

        GameObject* helloText = new GameObject();
        
        // Create and append "hello world" text component

        auto trc = new TextRendererComponent();
        helloText->appendComponent<TextRendererComponent> (trc);
        trc->text = "Hello, world!";
        trc->color = Colors::WHITE;

        // Center the text

        Transform* textTransform = helloText->getComponent<Transform> ();
        Vector2 textSize = trc->getSize();
        Vector2 newTextPos = Vector2(textSize.x / -2, textSize.y / 2);
        textTransform->setPosition(Vector3(newTextPos, 1));

        while (!win.shouldClose()) {
            win.fill(Colors::BLACK); // Reset the background each fr
            win.beginFrame();
            win.endFrame();
        }
    }
    
    terminate();

    return 0;
}
