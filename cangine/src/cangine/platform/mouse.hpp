#pragma once


#include "../types/types.hpp"

struct GLFWwindow;

namespace cangine {
    class Window;  // forward declaration

    /// @brief Mouse button identifiers, with named aliases for the common three.
    enum class MouseButton {
        Button1,            ///< First button.
        Button2,            ///< Second button.
        Button3,            ///< Third button.
        Button4,            ///< Fourth button.
        Button5,            ///< Fifth button.
        Button6,            ///< Sixth button.
        Button7,            ///< Seventh button.
        Button8,            ///< Eighth button.
        Left = Button1,     ///< Left button (alias of Button1).
        Right = Button2,    ///< Right button (alias of Button2).
        Middle = Button3    ///< Middle button (alias of Button3).
    };

    /// @brief The kind of mouse event that occurred.
    enum class MouseEventType {
        CursorMove,    ///< Cursor moved within the window.
        CursorEnter,   ///< Cursor entered the window.
        CursorLeave,   ///< Cursor left the window.
        MouseClick,    ///< A button was pressed.
        MouseRelease,  ///< A button was released.
        Scroll         ///< The scroll wheel moved.
    };

    /// @brief A mouse event carrying its type, button, position and scroll offset.
    struct MouseEvent {
        MouseEventType event; ///< The kind of event.
        MouseButton button;  ///< Button involved; only meaningful for click/release events.
        Vector2 pos;  ///< Cursor position at the time the event fired.
        Vector2 offset;  ///< Scroll offset; only meaningful for scroll events.
    };

    /**
     * @brief Singleton tracking mouse state and dispatching mouse events.
     *
     * Receives raw GLFW callbacks via @ref Window and exposes the current
     * cursor position.
     */
    class Mouse {
        private:
         Vector2 cursorPos; ///< Last known cursor position.
         /// @brief Handles a raw GLFW mouse button callback.
         void _mouseClickHandler(GLFWwindow* window, int button, int action, int mods);
         /// @brief Handles a raw GLFW cursor enter/leave callback.
         void _mouseEnterHandler(GLFWwindow* window, int entered);
         /// @brief Handles a raw GLFW cursor move callback.
         void _cursorMoveHandler(GLFWwindow* window);
         /// @brief Handles a raw GLFW scroll callback.
         void _scrollWheelInputHandler(GLFWwindow* window, Vector2 offset);
         /// @brief Dispatches a mouse event to listeners.
         void _fireEvent(const MouseEvent* event);
         static Mouse* instance; ///< Singleton instance.
         friend class Window;

        public:
        /**
         * @brief Returns the current cursor position.
         * @return Cursor position in window coordinates.
         */
        Vector2 getCursorPosition();
        /// @brief Constructs the mouse tracker.
        Mouse();
        /**
         * @brief Returns the singleton instance.
         * @return Pointer to the Mouse instance.
         */
        static Mouse* getInstance();

    };
}
