#pragma once

#include <iostream>

namespace cangine {
	/**
	 * @brief Wraps an OpenGL texture object.
	 *
	 * Can be created from an image file on disk or from a raw pixel buffer
	 * (e.g. a glyph atlas), then bound for sampling in a shader.
	 */
	class Texture {
	public:
		unsigned int ID;   ///< OpenGL texture object handle.
		std::string path;  ///< Path of the source image, if loaded from a file.
		/// @brief Constructs an empty texture without loading any data.
		Texture();
		/**
		 * @brief Constructs a texture from an image file path (not yet loaded).
		 * @param path Path to the image file.
		 */
		Texture(std::string path);
        /**
         * @brief Creates a texture from a raw pixel buffer.
         * @param width Width of the buffer in pixels.
         * @param height Height of the buffer in pixels.
         * @param data Pointer to the pixel data.
         * @param mono If @c true, treat the data as a single-channel image.
         */
        void createFromBuffer(int width, int height, unsigned char *data, bool mono = false);
        /**
         * @brief Replaces the texture's source image and reloads it.
         * @param path Path to the new image file.
         */
        void update(std::string path);
        /// @brief Loads the image at @ref path into GPU memory.
        void load();
		/// @brief Binds this texture for sampling.
		void bind();
		/// @brief Unbinds the current texture.
		void unbind();
	};
}
