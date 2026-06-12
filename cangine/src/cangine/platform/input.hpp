#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "key.hpp"

// Input class for cangine.
// currently only supports keyboard input.
//

namespace cangine {
    class Window;

    /**
     * @brief Static facade for keyboard input queries and event handlers.
     *
     * Offers both polling (is a key down this frame?) and callback registration
     * for key and character events. Events are fed in by @ref Window.
     */
    class Input {
    private:
        static inline std::map<size_t, std::function<void(std::string)>> charEventHandlers; ///< Registered character-event callbacks by id.
        static inline std::map<size_t, std::function<void(KeyEvent)>> keyEventHandlers;      ///< Registered key-event callbacks by id.
        static inline size_t nextID = 0; ///< Next id to hand out when registering a handler.
        /**
         * @brief Dispatches a character event to all registered handlers.
         * @param s The typed character(s) as UTF-8 text.
         */
        static void handleCharEvent(std::string s);
        /**
         * @brief Dispatches a key event to all registered handlers.
         * @param key The key involved.
         * @param eventType The kind of key event.
         */
        static void handleKeyEvent(Key key, KeyEventType eventType);
        friend class Window;

    public:
        /**
         * @brief Tests whether a key is currently held down.
         * @param key Key to query.
         * @return @c true while the key is held.
         */
        static bool getKey(Key key);
        /**
         * @brief Tests whether a key was pressed this frame.
         * @param key Key to query.
         * @return @c true only on the frame the key transitions to down.
         */
        static bool getKeyDown(Key key);
        /**
         * @brief Tests whether a key was released this frame.
         * @param key Key to query.
         * @return @c true only on the frame the key transitions to up.
         */
        static bool getKeyUp(Key key);

        /**
         * @brief Registers a callback invoked on every key event.
         * @param f Handler to register.
         * @return An id used to unregister the handler later.
         */
        static size_t registerKeyEventHandler(std::function<void(KeyEvent)> f);
        /**
         * @brief Unregisters a previously registered key-event handler.
         * @param id Id returned by @ref registerKeyEventHandler.
         */
        static void unregisterKeyEventHandler(size_t id);

        /**
         * @brief Registers a callback invoked on every character event.
         * @param f Handler to register.
         * @return An id used to unregister the handler later.
         */
        static size_t registerCharEventHandler(std::function<void(std::string)> f);
        /**
         * @brief Unregisters a previously registered character-event handler.
         * @param id Id returned by @ref registerCharEventHandler.
         */
        static void unregisterCharEventHandler(size_t id);
    };
}
