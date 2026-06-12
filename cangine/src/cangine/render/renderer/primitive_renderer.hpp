#pragma once

#include <vector>

#include "../../util/position.hpp"

#include "../../shapes/shapes.hpp"

#include "../vao.hpp"
#include "../vbo.hpp"
#include "../shader.hpp"

namespace cangine {

	/**
	 * @brief Batches and draws untextured geometric primitives.
	 *
	 * Accumulates triangle/rectangle vertices over a frame and submits them to
	 * the GPU as a single batch, using a supplied @ref Shader.
	 */
	class PrimitiveRenderer {
	private:
		static PrimitiveRenderer* instance; ///< Singleton instance.
		std::vector<Vertex> vertices;       ///< Accumulated vertices for the current frame.
		VAO vao;                            ///< Vertex array describing the layout.
		VBO vbo;                            ///< Vertex buffer holding the batched data.
		Shader* shader;                     ///< Shader used to draw the primitives.
	public:
		/**
		 * @brief Constructs the renderer with a shader.
		 * @param shader Shader used for drawing primitives.
		 */
		PrimitiveRenderer(Shader* shader);
		/// @brief Resets the batch at the start of a frame.
		void beginFrame();
		/// @brief Flushes the accumulated batch to the GPU.
		void endFrame();
		/**
		 * @brief Adds a triangle to the current batch.
		 * @param t Triangle to draw.
		 */
		void draw(const Triangle& t);
		/**
		 * @brief Adds a rectangle to the current batch.
		 * @param t Rectangle to draw.
		 */
		void draw(const Rectangle& t);
		/**
		 * @brief Replaces the shader used for drawing.
		 * @param shader New shader to use.
		 */
		void setShader(Shader* shader);
	};
}
