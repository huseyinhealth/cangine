#pragma once

#include <iostream>
#include <vector>
#include <optional>
#include <cstdint>

#include "../../engine/components/transform_component.hpp"

#include "../../shapes/shapes.hpp"

#include "../../types/types.hpp"

#include "../../util/color.hpp"
#include "../../util/position.hpp"

#include "../vao.hpp"
#include "../vbo.hpp"
#include "../ebo.hpp"
#include "../shader.hpp"
#include "../texture.hpp"


namespace cangine {

    /**
     * @brief Batched renderer for textured quads (rectangles).
     *
     * Accumulates up to @ref MAX_QUADS quads per batch and renders them with a
     * single draw call. Supports an optional bound texture and a monochrome
     * mode used for glyph rendering.
     */
    class QuadRenderer {
    private:
        bool currentMono = false; ///< Whether the bound texture is single-channel.

        static QuadRenderer* instance;          ///< Singleton instance.
        Texture* currentTexture = nullptr;       ///< Currently bound texture, if any.
        static constexpr uint32_t MAX_QUADS = 10000;          ///< Maximum quads per batch.
        static constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4; ///< Vertex capacity (4 per quad).
        static constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;  ///< Index capacity (6 per quad).

        std::vector<Vertex> vertices; ///< Accumulated vertices for the current batch.
        uint32_t quadCount = 0;       ///< Number of quads in the current batch.

        VAO vao;        ///< Vertex array describing the layout.
        VBO vbo;        ///< Vertex buffer holding the batched data.
        EBO ebo;        ///< Index buffer for the quads.
        Shader* shader; ///< Shader used to draw the quads.

    public:
        /**
         * @brief Constructs the renderer with a shader.
         * @param shader Shader used for drawing quads.
         */
        QuadRenderer(Shader* shader);

        /// @brief Resets the batch at the start of a frame.
        void beginFrame();
        /**
         * @brief Adds a transformed rectangle to the current batch.
         * @param transform Transform applied to the rectangle.
         * @param r Rectangle geometry to draw.
         * @param normalize If @c true, positions are normalized to clip space.
         */
        void draw(const Transform& transform, const Rectangle& r, bool normalize = true);
        /**
         * @brief Adds a rectangle to the current batch.
         * @param r Rectangle geometry to draw.
         * @param normalize If @c true, positions are normalized to clip space.
         */
        void draw(const Rectangle& r, bool normalize = true);
        /// @brief Submits the accumulated batch to the GPU.
        void flush();
        /**
         * @brief Binds a texture for subsequent quads, flushing if it changes.
         * @param t Texture to bind.
         * @param mono If @c true, treat the texture as single-channel.
         */
        void setTexture(Texture* t, bool mono = false);
        /**
         * @brief Replaces the shader used for drawing.
         * @param shader New shader to use.
         */
        void setShader(Shader* shader);
    };

}
