#include "inputfield.hpp"

using namespace cangine;

class InputFieldExample : public CangineApp {
    private:
        InputField* ifield;
        Button* submit;
        Label* label;

        std::function<void(std::string)> submitHandler = [this](std::string s) {
            if (this->ifield->getContent() == "") {
                this->ifield->setBorderColorFocus(Colors::RED);
                this->ifield->setPlaceHolderColor(Colors::RED);
                this->ifield->setBorderColor(Colors::RED);

                this->label->setText("Input Field is EMPTY!");
                this->label->setColor(Colors::RED);
                Transform::posFor(this->label, Vector3(this->label->getSize().x / -2, 70, 80));
                return;
            }
            this->label->setColor(Colors::WHITE);
            this->ifield->setBorderColorFocus(Colors::WHITE);
            this->ifield->setPlaceHolderColor(Colors::GRAY);
            this->ifield->setBorderColor(Color(30, 30, 30));
            this->label->setText("Hello, " + s + "!");
            Transform::posFor(this->label, Vector3(this->label->getSize().x / -2, 70, 80));
        };

       public : InputFieldExample() : CangineApp(800, 600, "cangine :: Hello World") {}

        void onFirstFrame() override {
            this->ifield = this->instantiateObject<InputField>();

            ifield->setPlaceholder("Name?");
            ifield->setOnReturn(submitHandler);

            Transform::posFor(ifield, Vector3(ifield->getWidth() / -2, 30, 80));

            this->submit = this->instantiateObject<Button>("Submit");
            this->submit->registerOnClickEvent([this](const MouseEvent* e) {
                this->submitHandler(this->ifield->getContent());
            });

            Transform::posFor(this->submit, Vector3(this->submit->getSize().x / -2, -20, 80));

            this->label = this->instantiateObject<Label>();
            this->label->setText("");
        }
};

int main() {
    std::cout << "Hello world\n";
    InputFieldExample().run();
    return 0;
}
