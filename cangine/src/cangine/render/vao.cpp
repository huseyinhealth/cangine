#include "vao.hpp"
#include <iostream>
#include <epoxy/gl.h>

namespace cangine {
    VAO::VAO() {
        glGenVertexArrays(1, &ID);
    }

    VAO::~VAO() {
        glDeleteVertexArrays(1, &ID);
    }

    void VAO::bind() const {
        glBindVertexArray(ID);
    }

    void VAO::unbind() const {
        glBindVertexArray(0);
    }

    void VAO::linkAttrib(
        const VBO& vbo,
        GLuint layoutLocation,
        GLint count,
        GLsizei stride,
        uintptr_t offset
    ) {
        bind();
        vbo.bind();

        glVertexAttribPointer(
            layoutLocation,
            count,
            GL_FLOAT,
            GL_FALSE,
            stride,
            (void*)offset
        );
        glEnableVertexAttribArray(layoutLocation);
    }
}
