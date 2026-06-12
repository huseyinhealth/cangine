#include "render_system.hpp"

#include <cassert>

namespace cangine {
    RenderSystem *RenderSystem::instance = nullptr;

    RenderSystem::RenderSystem (Shader &shader) {
        assert(instance == nullptr);
        this->primitiveRenderer = new PrimitiveRenderer(&shader);
        this->quadRenderer = new QuadRenderer(&shader);
        this->textRenderer = new TextRenderer();
        instance = this;
    }
    RenderSystem::~RenderSystem() {
        delete this->primitiveRenderer;
        delete this->quadRenderer;
        delete this->textRenderer;
    }
    
    void RenderSystem::setShader(Shader &shader) {
        this->primitiveRenderer->setShader(&shader);
        this->quadRenderer->setShader(&shader);
    }
    RenderSystem* RenderSystem::getInstance()
    {
	    return instance;
    }
}
