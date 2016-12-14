#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <SOIL.h>
#include "Shader.h"

namespace engine {

	class Texture
	{
	protected:
		GLuint texture;

	public:
		Texture() {}
		Texture(std::string name) {
			Create(name);
		}
		virtual ~Texture() {
			glDeleteTextures(1, &texture);
		}

		virtual void Use() {
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		virtual void UnUse() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	private:
		void Create(std::string name);
		
	};

}