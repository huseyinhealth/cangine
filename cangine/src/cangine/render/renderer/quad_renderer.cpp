#include "../renderer/quad_renderer.hpp"
#include <epoxy/gl.h>
#include "quad_renderer.hpp"
#include <cassert>
#include <cmath>

namespace cangine {

    QuadRenderer* QuadRenderer::instance = nullptr;

    QuadRenderer::QuadRenderer(Shader* shader)
        : shader(shader)
    {
        assert(instance == nullptr && "Access QuadRenderer from `RenderSystem::quadRenderer`!");
        vertices.reserve(MAX_QUADS * 4);

        vao.bind();
        vbo.bind();
        ebo.bind();

        glBufferData(
            GL_ARRAY_BUFFER,
            MAX_QUADS * 4 * sizeof(Vertex),
            nullptr,
            GL_DYNAMIC_DRAW
        );

        // position (location = 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, x)
        );

        // color (location = 1)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, color)
        );

        // texture (location = 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, uv)
        );

        std::vector<uint32_t> indices;
        indices.reserve(MAX_QUADS * 6);

        uint32_t offset = 0;
        for (uint32_t i = 0; i < MAX_QUADS; i++) {
            indices.push_back(offset + 0);
            indices.push_back(offset + 1);
            indices.push_back(offset + 2);

            indices.push_back(offset + 0);
            indices.push_back(offset + 2);
            indices.push_back(offset + 3);

            offset += 4;
        }

        ebo.update(indices);
    }

    void QuadRenderer::beginFrame() {
        vertices.clear();
        quadCount = 0;
    }

    void QuadRenderer::draw(const Transform& transform, const Rectangle& r, bool normalize) {
        if (quadCount >= MAX_QUADS) {
            flush();
            beginFrame();
        }

        Vector3 pos = transform.getPosition();
        Vector3 rot = transform.getRotation();
        Vector3 scl = transform.getScale();

        float hw = (r.width  * scl.x) * 0.5f;
        float hh = (r.height * scl.y) * 0.5f;

        float radians = rot.z * 3.14159265f / 180.0f;
        float c = std::cos(radians);
        float s = std::sin(radians);

        auto rotatePoint = [&](float x, float y) -> Vector3 {
            float rx = x * c - y * s;
            float ry = x * s + y * c;
            return { pos.x + rx, pos.y + ry, pos.z };
        };

        Vector3 A = rotatePoint(-hw, -hh);
        Vector3 B = rotatePoint( hw, -hh);
        Vector3 C = rotatePoint( hw,  hh);
        Vector3 D = rotatePoint(-hw,  hh);

        if (normalize) {
            vertices.push_back(Vertex(Position::normalize(A), r.color, { 0.0f, 0.0f }));
            vertices.push_back(Vertex(Position::normalize(B), r.color, { 1.0f, 0.0f }));
            vertices.push_back(Vertex(Position::normalize(C), r.color, { 1.0f, 1.0f }));
            vertices.push_back(Vertex(Position::normalize(D), r.color, { 0.0f, 1.0f }));
        } else {
            vertices.push_back(Vertex(A, r.color, { 0.0f, 0.0f }));
            vertices.push_back(Vertex(B, r.color, { 1.0f, 0.0f }));
            vertices.push_back(Vertex(C, r.color, { 1.0f, 1.0f }));
            vertices.push_back(Vertex(D, r.color, { 0.0f, 1.0f }));
        }

        quadCount++;
    }

    void QuadRenderer::draw(const Rectangle& r, bool normalize) {
        if (quadCount >= MAX_QUADS) {
            flush();
            beginFrame();
        }

        Vector3 A = r.pos;
        Vector3 B = { r.pos.x + r.width, r.pos.y, r.pos.z };
        Vector3 C = { B.x, r.pos.y + r.height, r.pos.z };
        Vector3 D = { r.pos.x, C.y, r.pos.z };

        if (normalize) {
            vertices.push_back(Vertex(Position::normalize(A), r.color, { 0.0f, 0.0f }));
            vertices.push_back(Vertex(Position::normalize(B), r.color, { 1.0f, 0.0f }));
            vertices.push_back(Vertex(Position::normalize(C), r.color, { 1.0f, 1.0f }));
            vertices.push_back(Vertex(Position::normalize(D), r.color, { 0.0f, 1.0f }));
        } else {
            vertices.push_back(Vertex(A, r.color, { 0.0f, 0.0f }));
            vertices.push_back(Vertex(B, r.color, { 1.0f, 0.0f }));
            vertices.push_back(Vertex(C, r.color, { 1.0f, 1.0f }));
            vertices.push_back(Vertex(D, r.color, { 0.0f, 1.0f }));
        }

        quadCount++;
    }

    void QuadRenderer::flush() {
        if (this->quadCount == 0)
            return;

        this->vao.bind();
        this->vbo.bind();
        this->ebo.bind();

        this->vbo.update(
            this->vertices.data(),
            this->vertices.size() * sizeof(Vertex)
        );

        this->shader->use();


        if (currentTexture) {
            this->shader->setBool("uUseTexture", true);
            glActiveTexture(GL_TEXTURE0);
            this->currentTexture->bind();
            this->shader->setInt("uTexture", 0);
        }
        else {
            shader->setBool("uUseTexture", false);
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDrawElements(
            GL_TRIANGLES,
            quadCount * 6,
            GL_UNSIGNED_INT,
            nullptr
        );
    }

    void QuadRenderer::setTexture(Texture* t, bool mono) {
        if (t != this->currentTexture || mono != this->currentMono)
        {
            this->flush();
            this->beginFrame();
            this->currentTexture = t;
            if (this->shader){
                shader->use();
                this->shader->setBool("uMono", mono);
                this->currentMono = mono;
            }
        }
    }

    void QuadRenderer::setShader(Shader *shader) {
        this->shader->unbind();
        this->shader = shader;
    }
}

