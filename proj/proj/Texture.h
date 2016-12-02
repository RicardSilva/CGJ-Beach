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
		void Create(std::string name);
		void Destroy();

		void Use() {
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		void UnUse() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	};

}