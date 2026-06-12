#include "ebo.hpp"
#include <epoxy/gl.h>

namespace cangine {
	cangine::EBO::EBO()	{
		glGenBuffers(1, &this->ID);
	}

	void EBO::update(const std::vector<unsigned int> &indices) {
		this->bind();
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			indices.size() * sizeof(unsigned int),
			indices.data(),
			GL_STATIC_DRAW
		);
	}

	void EBO::bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
	}	
}
