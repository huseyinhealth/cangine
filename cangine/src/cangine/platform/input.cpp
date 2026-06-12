#include <map>

#include "input.hpp"
#include "window.hpp"

namespace cangine {
    void Input::handleCharEvent(std::string s) {
        for (auto const& [_id, handler] : charEventHandlers) {
            if (handler) {
                handler(s);
            }
        }
    }

    void Input::handleKeyEvent(Key key, KeyEventType eventType) {
        KeyEvent event = {
            key,
            eventType
        };

        for (auto const& [_id, handler] : keyEventHandlers) {
            handler(event);
        }
    }

    bool Input::getKey(Key key) {
        return Window::keysPressed.find(key) != Window::keysPressed.end();
    }

    bool Input::getKeyDown(Key key) {
        return getKey(key) && Window::prevKeysPressed.find(key) == Window::prevKeysPressed.end();
    }

    bool Input::getKeyUp(Key key) {
        return !getKey(key) && Window::prevKeysPressed.find(key) != Window::prevKeysPressed.end();           
    }

    size_t Input::registerKeyEventHandler(std::function<void(KeyEvent)> f) {
        if (!f) return 0;

        size_t id = ++nextID;
        keyEventHandlers[id] = f;
        return id;
    }

    void Input::unregisterKeyEventHandler(size_t id) {
        keyEventHandlers.erase(id);
    }

    size_t Input::registerCharEventHandler(std::function<void(std::string)> f) {
        if (!f) return 0;

        size_t id = ++nextID;
        charEventHandlers[id] = f;
        return id;
    }

    void Input::unregisterCharEventHandler(size_t id) {
        charEventHandlers.erase(id);
    }


}
