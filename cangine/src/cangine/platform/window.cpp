#include "window.hpp"
#include <cassert>
#include <epoxy/gl.h>
#include <GLFW/glfw3.h>

#include "../render/renderer/render_system.hpp"
#include "../engine/object_manager.hpp"

#include "input.hpp"

namespace cangine {
	Mouse* mouse = nullptr;
	std::unordered_set<Key> Window::keysPressed;
	std::unordered_set<Key> Window::prevKeysPressed;

	void resizeCallback(GLFWwindow* window, int w, int h) {
		Window* instance = (Window*)glfwGetWindowUserPointer(window);

		if (instance) {
			instance->onResize(w, h);
		}
	}

	Window* Window::instance = nullptr;

	void Window::_processInput(GLFWwindow *win, int key, int scanCode, int action, int mods) {
        Key cangineKey = translateGLFWKey(key);
        KeyEventType ket = getKeyEventType(action);

        if (ket == KeyEventType::Down)
            keysPressed.insert(cangineKey);
        else if (ket == KeyEventType::Up)
            keysPressed.erase(cangineKey);

		Input::handleKeyEvent(cangineKey, ket);
	}

	void Window::_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		mouse->_mouseClickHandler(window, button, action, mods);
	}

	void Window::_mouseEnterCallback(GLFWwindow* window, int e) {
		mouse->_mouseEnterHandler(window, e);
	}

    void Window::_mouseMoveCallback(GLFWwindow *window, double _x, double _y) {
		mouse->_cursorMoveHandler(window);
    }

    void Window::_characterCallback(GLFWwindow* window, unsigned int codepoint) {
		std::string s;

		if (codepoint <= 0x7F) {
			s += (char)codepoint;
		} else if (codepoint <= 0x7FF) {
			s += (char)(0xC0 | (codepoint >> 6));
			s += (char)(0x80 | (codepoint & 0x3F));
		} else if (codepoint <= 0xFFFF) {
			s += (char)(0xE0 | (codepoint >> 12));
			s += (char)(0x80 | ((codepoint >> 6) & 0x3F));
			s += (char)(0x80 | (codepoint & 0x3F));
		} else if (codepoint <= 0x10FFFF) {
			s += (char)(0xF0 | (codepoint >> 18));
			s += (char)(0x80 | ((codepoint >> 12) & 0x3F));
			s += (char)(0x80 | ((codepoint >> 6) & 0x3F));
			s += (char)(0x80 | (codepoint & 0x3F));
		}

        Input::handleCharEvent(s);
    }

    void Window::_scrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
		Vector2 offset((float) xOffset, (float) yOffset);
		mouse->_scrollWheelInputHandler(window, offset);
    }

    Window::Window(int width, int height, std::string title, bool fullscreen) {
		assert(instance == nullptr && "Access via: Window::getInstance()");
		this->lastTime = glfwGetTime();
		this->frameCount = 0;

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		this->main = glfwCreateWindow(width, height, title.c_str(), (fullscreen ? glfwGetPrimaryMonitor() : NULL), NULL);

		if (this->main == NULL) {
			glfwTerminate();
			throw Exception::CangineException("Failed to create GLFW window\n");
		}
		glfwMakeContextCurrent(this->main);
		
		this->loadGLAD();

		mouse = new Mouse();

		glfwSetWindowUserPointer(this->main, this);
		
		glfwSetFramebufferSizeCallback(this->main, resizeCallback);
		
		glfwSetKeyCallback(this->main, _processInput);
		glfwSetCharCallback(this->main, _characterCallback);
		
		glfwSetMouseButtonCallback(this->main, _mouseButtonCallback);
		glfwSetCursorEnterCallback(this->main, _mouseEnterCallback);
		glfwSetCursorPosCallback(this->main, this->_mouseMoveCallback);
		glfwSetScrollCallback(this->main, this->_scrollCallback);

		int fbW, fbH;
		glfwGetFramebufferSize(this->main, &fbW, &fbH);
		glViewport(0, 0, fbW, fbH);
		this->dimensions = { fbW, fbH };
		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwSwapInterval(1);


		instance = this;
		Position::window_instance = instance;

		std::cout << "Window is initialized without any problem.\n";
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	}

    Window::~Window()
    {
		delete mouse;
		instance = nullptr;
    }

    bool Window::shouldClose() {
		return glfwWindowShouldClose(this->main);
	}

	void Window::pollEvents() {
		glfwPollEvents();
	}

	void Window::loadGLAD()	{
		// epoxy loads GL functions automatically on first use
	}

	void cangine::Window::swapBuffers()
	{
		glfwSwapBuffers(this->main);
	}

	void Window::onResize(int w, int h) {
		int fbW, fbH;
		glfwGetFramebufferSize(this->main, &fbW, &fbH);
		glViewport(0, 0, fbW, fbH);
		this->dimensions = { fbW, fbH };
	}

	void cangine::Window::setTitle(std::string title) {
		glfwSetWindowTitle(this->main, title.c_str());
	}

	void Window::fill(const Color& c) {
		glClearColor(c.r, c.g, c.b, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void Window::beginFrame() {
        double currentTime = glfwGetTime();

		// calculate delta time

		this->deltaTime = static_cast<float>(currentTime - this->lastFrameTime);
    	this->lastFrameTime = currentTime;
		
		// calculate fps
        
		this->frameCount++;
        if (currentTime - this->lastTime >= 1.0) {
			this->fps = frameCount;
            this->frameCount = 0;
            this->lastTime += 1.0;
        }

		this->instantFps = 1.0f / deltaTime;
		
		// begin renderer frame
		
		RenderSystem::getInstance()->primitiveRenderer->beginFrame();
		RenderSystem::getInstance()->quadRenderer->beginFrame();
		
		// run update event
		this->pollEvents();
		ObjectManager::getInstance()->updateAll();
	}

    void Window::endFrame() {
		RenderSystem::getInstance()->primitiveRenderer->endFrame();
		RenderSystem::getInstance()->quadRenderer->flush();

        prevKeysPressed = keysPressed;
		
		this->swapBuffers();
    }

    int Window::getFPS() {
		return this->fps;
	}

    int Window::getInstantFPS()
    {
        return this->instantFps;
    }

    double Window::getDeltaTime() {
        return this->deltaTime;
    }

	Window* Window::getInstance() {
		return instance;
	}
}
