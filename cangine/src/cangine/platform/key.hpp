#pragma once
// remapping keys for better accessibility.

namespace cangine {
    /**
     * @brief Platform-independent keyboard key identifiers.
     *
     * GLFW key codes are translated into these values so engine code does not
     * depend on the windowing backend.
     */
    enum class Key {
        Unknown = -1, ///< Unrecognized or unmapped key.

        // charactes
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        // numbers
        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

        // function
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,

        // direction
        Up, Down, Left, Right,

        // control keys
        Escape, Enter, Tab, Space, Backspace, Insert, Delete, Pause, CapsLock, ScrollLock, NumLock, PrintScreen, Menu,

        // shift / ctrl / alt
        LeftShift, RightShift, LeftCtrl, RightCtrl, LeftAlt, RightAlt, LeftSuper, RightSuper, 
        ContextMenu,

        // numpad
        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
        NumpadDecimal, NumpadDivide, NumpadMultiply, NumpadSubtract, NumpadAdd, NumpadEnter, NumpadEqual,

        // other
        Minus, Equal, LeftBracket, RightBracket, Backslash, Semicolon, Apostrophe, Comma, Period, Slash, GraveAccent,
    };

    /// @brief The kind of keyboard event that occurred.
    enum class KeyEventType {
        Down,    ///< Key was pressed this frame.
        Up,      ///< Key was released this frame.
        Repeat,  ///< Key is held and generating auto-repeat events.
        Unknown  ///< Unrecognized event type.
    };

    /// @brief A keyboard event: which key and what happened to it.
    struct KeyEvent {
        Key key;           ///< The key involved in the event.
        KeyEventType type; ///< The kind of event.
    };

    /**
     * @brief Translates a raw GLFW key code into a @ref Key.
     * @param key GLFW key code.
     * @return The corresponding @ref Key, or @ref Key::Unknown if unmapped.
     */
    Key translateGLFWKey(int key);
    /**
     * @brief Translates a raw GLFW key action into a @ref KeyEventType.
     * @param glfwKeyEvent GLFW key action code.
     * @return The corresponding @ref KeyEventType.
     */
    KeyEventType getKeyEventType(int glfwKeyEvent);
}
