#include "vbo.hpp"
#include <epoxy/gl.h>

namespace cangine {
    VBO::VBO() {
        glGenBuffers(1, &ID);
        this->bind();
    }

    VBO::~VBO() {
        glDeleteBuffers(1, &ID);
    }

    void VBO::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
    }

    void VBO::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void VBO::update(const void* data, size_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}
