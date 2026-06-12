#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture.hpp"
#include <epoxy/gl.h>

namespace cangine {
	Texture::Texture() {
		glGenTextures(1, &this->ID);
	}

	Texture::Texture(std::string path) {
		glGenTextures(1, &this->ID);
		this->update(path);
	}

	void Texture::createFromBuffer(int width, int height, unsigned char* data, bool mono) {
		this->bind();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (mono) {
			// single-channel for FreeType glyphs
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,       // internal format
				width,
				height,
				0,
				GL_RED,       // format
				GL_UNSIGNED_BYTE,
				data
			);
		} else {
			// Normal RGBA texture
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				data
			);
		}

		this->unbind();
	}

	void Texture::update(std::string path) {
		stbi_set_flip_vertically_on_load(true);
		this->path = path;
		this->load();
	}
	
	void Texture::bind() {
		glBindTexture(GL_TEXTURE_2D, this->ID);
	}

	void Texture::unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::load() {
		this->bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(this->path.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

		if (data) {
			GLenum format = GL_RGB;

			// std::cout << "color channel: " << nrChannels << "\n";

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << "--- Loaded: " << this->path << " ---\n";
		} else {
			std::cout << "--- Failed to load texture: " << this->path<< " ---\n" ;
			std::cout << stbi_failure_reason() << "\n";
		}

		stbi_image_free(data);
		this->unbind();
	}
}
