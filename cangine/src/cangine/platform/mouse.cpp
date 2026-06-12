#include "mouse.hpp"
#include "window.hpp"
#include "../engine/object_manager.hpp"
#include <GLFW/glfw3.h>

#include <cassert>

namespace cangine {
    Mouse* Mouse::instance = nullptr;

    Vector2 _getCursorPos(GLFWwindow* window) {
        Window* cangineWindow = Window::getInstance();
        Vector2 cursorPos;

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        float centeredX = (float)xpos - (cangineWindow->dimensions.width / 2.0f);
        float centeredY = -((float)ypos - (cangineWindow->dimensions.height / 2.0f));

        return Vector2(centeredX, centeredY);
    }

    void Mouse::_mouseClickHandler(GLFWwindow* window, int glfwButton, int action, int mods) {        
        MouseButton button = (MouseButton) glfwButton;

        MouseEventType eventType = 
            action == GLFW_PRESS ? MouseEventType::MouseClick : MouseEventType::MouseRelease;
            
        Vector2 cursorPos = _getCursorPos(window);

        MouseEvent event = {
            eventType,
            button,
            cursorPos,
            Vector2()
        };

        this->_fireEvent(&event);
    }

    void Mouse::_mouseEnterHandler(GLFWwindow *window, int entered) {
        Vector2 cursorPos = _getCursorPos(window);
        MouseEventType eventType =
            entered ? MouseEventType::CursorEnter : MouseEventType::CursorLeave;

        MouseEvent event = {
            eventType,
            (MouseButton) 0,
            cursorPos,
            Vector2()
        };

        this->_fireEvent(&event);
    }

    void Mouse::_cursorMoveHandler(GLFWwindow* window) {
        MouseEventType eventType = MouseEventType::CursorMove;
        Vector2 cursorPos = _getCursorPos(window);

        MouseEvent event = {
            eventType,
            (MouseButton) 0,
            cursorPos,
            Vector2()
        };

        this->cursorPos = cursorPos;
        this->_fireEvent(&event);
    }

    void Mouse::_scrollWheelInputHandler(GLFWwindow *window, Vector2 offset) {
        MouseEventType eventType = MouseEventType::Scroll;
        Vector2 cursorPos = _getCursorPos(window);

        MouseEvent event = {
            eventType,
            (MouseButton) 0,
            cursorPos,
            offset
        };

        this->_fireEvent(&event);
    }

    void Mouse::_fireEvent(const MouseEvent* event) {
        ObjectManager* objMan = ObjectManager::getInstance();
        if (objMan == nullptr) return;

        objMan->fireMouseEvent(event);
    }

    Vector2 Mouse::getCursorPosition() {
        return this->cursorPos;
    }

    Mouse::Mouse() {
        assert(instance == nullptr && "Acces via: Mouse::getInstance();");
        instance = this;
    }

    Mouse *Mouse::getInstance() {
        return instance;
    }
}
