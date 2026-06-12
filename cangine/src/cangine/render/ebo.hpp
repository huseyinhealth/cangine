#pragma once

#include <vector>

namespace  cangine {
	/**
	 * @brief Wraps an OpenGL Element Buffer Object (index buffer).
	 *
	 * Stores the index data that tells the GPU which vertices form each
	 * primitive, allowing vertices to be reused across triangles.
	 */
	class EBO {
	private:
		unsigned int ID; ///< OpenGL buffer object handle.
	public:
		/// @brief Generates the underlying OpenGL buffer object.
		EBO();
		/**
		 * @brief Uploads index data to the buffer.
		 * @param indices Vertex indices defining the primitives.
		 */
		void update(const std::vector<unsigned int> &indices);
		/// @brief Binds the buffer as the current element array buffer.
		void bind();
	};
}