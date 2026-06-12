#pragma once

#include <iostream>
#include <algorithm>
#include <unordered_set>

#include "../util/position.hpp"
#include "../types/types.hpp"
#include "../util/color.hpp"

#include "../engine/exception.hpp"

#include "key.hpp"
#include "mouse.hpp"

struct GLFWwindow;

namespace cangine {
	/**
	 * @brief GLFW framebuffer resize callback.
	 * @param window Window that was resized.
	 * @param w New framebuffer width in pixels.
	 * @param h New framebuffer height in pixels.
	 */
	void resizeCallback(GLFWwindow* window, int w, int h);

	/**
	 * @brief Owns the application window, OpenGL context and the frame loop.
	 *
	 * Wraps a GLFW window: creates the context, pumps input/events, tracks
	 * frame timing (FPS and delta time) and exposes per-frame begin/end hooks.
	 * Implemented as a singleton accessible via @ref getInstance.
	 */
	class Window {
	private:
	double lastTime;            ///< Timestamp used for the per-second FPS counter.
	int frameCount;             ///< Frames counted in the current second.
	int fps;                    ///< Frames rendered in the last full second.
	int instantFps;             ///< FPS estimated from the latest frame's delta time.
	double lastFrameTime = 0.0; ///< Timestamp of the previous frame.
	float deltaTime = 0.0f;     ///< Seconds elapsed since the previous frame.
	GLFWwindow* main;           ///< Underlying GLFW window handle.
	static Window* instance;    ///< Singleton instance.
	/// @brief Raw GLFW mouse button callback, forwarded to @ref Mouse.
	static void _mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	/// @brief Raw GLFW cursor enter/leave callback, forwarded to @ref Mouse.
	static void _mouseEnterCallback(GLFWwindow *window, int e);
	/// @brief Raw GLFW cursor move callback, forwarded to @ref Mouse.
	static void _mouseMoveCallback(GLFWwindow *window, double _x, double _y);

    public:
	/**
	 * @brief Raw GLFW character input callback, forwarded to @ref Input.
	 * @param window Window receiving input.
	 * @param codepoint Unicode codepoint typed.
	 */
	static void _characterCallback(GLFWwindow* window, unsigned int codepoint);
	static std::unordered_set<Key> keysPressed;     ///< Keys held this frame.
	static std::unordered_set<Key> prevKeysPressed; ///< Keys held the previous frame (for edge detection).
	Dimensions dimensions; ///< Current window size in pixels.
	/**
	 * @brief Creates the window and OpenGL context.
	 * @param width Initial width in pixels.
	 * @param height Initial height in pixels.
	 * @param title Window title.
	 * @param fullscreen Whether to open in fullscreen mode.
	 */
	Window(int width, int height, std::string title, bool fullscreen = false);
	/// @brief Destroys the window and releases GLFW resources.
	~Window();
	/**
	 * @brief Reports whether the window has been asked to close.
	 * @return @c true if the window should close.
	 */
	bool shouldClose();
	/// @brief Raw GLFW keyboard callback, forwarded to @ref Input.
	static void _processInput(GLFWwindow* win, int key, int scanCode, int action, int mods);
	/// @brief Raw GLFW scroll callback, forwarded to @ref Mouse.
	static void _scrollCallback(GLFWwindow *window, double xOffset, double yOffset);
	/// @brief Swaps the front and back framebuffers.
	void swapBuffers();
	/// @brief Processes pending window and input events.
	void pollEvents();
	/// @brief Loads OpenGL function pointers via GLAD.
	void loadGLAD();
	/**
	 * @brief Updates cached state in response to a resize.
	 * @param w New width in pixels.
	 * @param h New height in pixels.
	 */
	void onResize(int w, int h);
	/**
	 * @brief Sets the window title.
	 * @param title New title text.
	 */
	void setTitle(std::string title);
	/**
	 * @brief Clears the framebuffer to a solid color.
	 * @param c Fill color.
	 */
	void fill(const Color &c);
	/// @brief Begins a frame: updates timing and prepares for rendering.
	void beginFrame();
	/// @brief Ends a frame: swaps buffers and polls events.
	void endFrame();
	/**
	 * @brief Returns the frames-per-second over the last full second.
	 * @return Averaged FPS.
	 */
	int getFPS();
	/**
	 * @brief Returns the instantaneous FPS from the latest frame.
	 * @return Instantaneous FPS.
	 */
	int getInstantFPS();
	/**
	 * @brief Returns the time elapsed since the previous frame.
	 * @return Delta time in seconds.
	 */
	double getDeltaTime();
	/**
	 * @brief Returns the singleton instance.
	 * @return Pointer to the Window instance.
	 */
	static Window* getInstance();
	};
}
