#pragma once
#include <cstddef>

namespace cangine {
    /**
     * @brief Wraps an OpenGL Vertex Buffer Object.
     *
     * Holds raw vertex data on the GPU. Use @ref VAO to describe how this
     * data is laid out for the shader.
     */
    class VBO {
    private:
        unsigned int ID; ///< OpenGL buffer object handle.
    public:
        // data: Ham vertex verisi, size: verinin byte cinsinden boyutu
        /// @brief Generates the underlying OpenGL buffer object.
        VBO();
        /// @brief Deletes the underlying OpenGL buffer object.
        ~VBO();

        /// @brief Binds the buffer as the current array buffer.
        void bind() const;
        /// @brief Unbinds the current array buffer.
        void unbind() const;
        /**
         * @brief Uploads raw vertex data to the buffer.
         * @param data Pointer to the raw vertex data.
         * @param size Size of the data in bytes.
         */
        void update(const void* data, size_t size);
    };
}
