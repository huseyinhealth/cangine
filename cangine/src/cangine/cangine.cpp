#include "cangine.hpp"
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <miniaudio.h>

namespace cangine {
    void init() {
        std::cout << "Hello from cangine!\n";
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef CANGINE_DEBUG
            std::cout << "DEBUG: initialized.\n";
        #endif
    }

    void terminate() {
        glfwTerminate();
        #ifdef CANGINE_DEBUG
            std::cout << "DEBUG: terminated program.\n";
        #endif
    }

    void CangineApp::initShader(std::string name) {
        if (name == "default") {
            this->shader = Shader::fromSource(CANGINE_DEFAULT_VERT_SRC, CANGINE_DEFAULT_FRAG_SRC);
        } else {
            this->shader = new Shader(name);
        }

        assert(this->shader->valid && "Invalid shader program.");
    }

    void CangineApp::initRenderSystem() {
        this->_rs = new RenderSystem(*this->shader);
    }

    void CangineApp::initObjectManager() {
        this->_om = new ObjectManager();
    }

    void CangineApp::initAudioManager() {
        this->audioMan = new AudioManager();
    }

    void CangineApp::run() {
        this->onFirstFrame();
        
        while (!this->window->shouldClose()) {
            double deltaTime = this->window->getDeltaTime();
            this->window->fill(this->windowFillColor);
            
            this->window->beginFrame();
            
            this->onFrame(deltaTime);

            this->window->endFrame();
        }
    }
    
    CangineApp::CangineApp(
        int windowWidth,
        int windowHeight,
        std::string windowTitle,
        bool fullscreen,
        Color windowFillColor,
        std::string defaultShaderName
    ) {
        init();

        this->window = new Window(windowWidth, windowHeight, windowTitle, fullscreen);
        this->windowFillColor = windowFillColor;

        this->initShader(defaultShaderName);
        this->initRenderSystem();
        this->initObjectManager();
        this->initAudioManager();
    }
    CangineApp::~CangineApp() {
        delete this->_om;
        delete this->_rs;
        delete this->shader;
        delete this->window;
        delete this->audioMan;

        this->_om = nullptr;
        this->_rs = nullptr;
        this->shader = nullptr;
        this->window = nullptr;
        this->audioMan = nullptr;

        terminate();
    }
}
