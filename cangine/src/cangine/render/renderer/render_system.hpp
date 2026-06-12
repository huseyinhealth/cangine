#pragma once

#include "quad_renderer.hpp"
#include "primitive_renderer.hpp"
#include "text_renderer.hpp"
#include "../shader.hpp"

namespace cangine {
	/**
	 * @brief Singleton aggregating the engine's renderers.
	 *
	 * Owns the @ref QuadRenderer, @ref PrimitiveRenderer and @ref TextRenderer
	 * and provides a single access point for submitting draw work.
	 */
	class RenderSystem {
	private:
		static RenderSystem* instance; ///< Singleton instance.
	public:
		QuadRenderer* quadRenderer;           ///< Renderer for textured quads.
		PrimitiveRenderer* primitiveRenderer; ///< Renderer for untextured primitives.
		TextRenderer* textRenderer;           ///< Renderer for text.

		/**
		 * @brief Constructs the render system and its sub-renderers.
		 * @param shader Default shader shared by the renderers.
		 */
		RenderSystem(Shader& shader);
		/// @brief Destroys the owned renderers.
		~RenderSystem();
        /**
         * @brief Sets the shader on the owned renderers.
         * @param shader Shader to apply.
         */
        void setShader(Shader &shader);
		/**
		 * @brief Returns the singleton instance.
		 * @return Pointer to the RenderSystem instance.
		 */
		static RenderSystem* getInstance();

	};

    /// @brief Identifies which primitive renderer backend to use.
    enum RendererType {
		QUADRENDERER,     ///< Textured quad renderer.
		PRIMITIVERENDERER ///< Untextured primitive renderer.
	};
}