#pragma once
#include "vbo.hpp"
#include <cstdint>

namespace cangine {
    /**
     * @brief Wraps an OpenGL Vertex Array Object.
     *
     * Records the vertex attribute layout (which @ref VBO feeds which shader
     * input and how) so it can be rebound with a single call.
     */
    class VAO {
    private:
        unsigned int ID; ///< OpenGL vertex array object handle.
    public:
        /// @brief Generates the underlying OpenGL vertex array object.
        VAO();
        /// @brief Deletes the underlying OpenGL vertex array object.
        ~VAO();

        /**
         * @brief Links a vertex attribute from a VBO into this VAO.
         * @param vbo Buffer supplying the attribute data.
         * @param layoutLocation Shader attribute location index.
         * @param count Number of components per attribute (e.g. 3 for a vec3).
         * @param stride Byte offset between consecutive attributes.
         * @param offset Byte offset of the first component within the vertex.
         */
        void linkAttrib(
            const VBO& vbo,
            unsigned int layoutLocation,
            int count,
            int stride,
            uintptr_t offset
        );

        /// @brief Binds this vertex array object.
        void bind() const;
        /// @brief Unbinds the current vertex array object.
        void unbind() const;
    };
}
