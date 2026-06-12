#include "inputfield_object.hpp"

#include "../../components/ui/inputfield_component.hpp"

namespace cangine {
    InputField::InputField() : GameObject() {
        this->ifc = this->newComponent<InputFieldComponent>();
    }
}
