#include "primitive_renderer.hpp"
#include <epoxy/gl.h>

namespace cangine {
    PrimitiveRenderer* PrimitiveRenderer::instance = nullptr;
    PrimitiveRenderer::PrimitiveRenderer(Shader* shader) {
        assert(instance == nullptr && "Access PrimitiveRenderer from `RenderSystem::primitiveRenderer`!");
        vao.bind();
        vbo.bind();

        this->shader = shader;

        glBufferData(
            GL_ARRAY_BUFFER,
            3000 * sizeof(Vertex),
            nullptr,
            GL_DYNAMIC_DRAW
        );

        vao.linkAttrib(
            vbo,
            0,                  // location
            3,                  // x y z
            sizeof(Vertex),
            0
        );

        vao.linkAttrib(
            vbo,
            1,
            4, // RGBA
            sizeof(Vertex),
            offsetof(Vertex, color)
        );

        vao.unbind();
    }


    void PrimitiveRenderer::beginFrame() {
        vertices.clear();
    }


    void PrimitiveRenderer::endFrame() {
        vao.bind();
        vbo.bind();

        vbo.update(vertices.data(), vertices.size() * sizeof(Vertex));
        this->shader->use();
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        vertices.clear();
    }

    void PrimitiveRenderer::draw(const Triangle& t) {
        for (const Vertex &vertex : t.vertices) {
            vertices.push_back(Position::normalize(vertex));
        }
    }

    void PrimitiveRenderer::draw(const Rectangle& t) {
        std::array<Triangle, 2> tris = rectToTriangle(t);
        for (const Triangle &tri : tris) {
            draw(tri);
        }
    }
    void PrimitiveRenderer::setShader(Shader *shader) {
        this->shader->unbind();
        this->shader = shader;
    }
}
