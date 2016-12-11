#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <SOIL.h>
#include "Shader.h"

namespace engine {

	class Texture
	{
		GLuint texture;

	public:
		Texture(std::string name) {
			Create(name);
		}
		virtual ~Texture() {
			glDeleteTextures(1, &texture);
		}

		void Use() {
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		void UnUse() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Create(std::string name);
	};

}