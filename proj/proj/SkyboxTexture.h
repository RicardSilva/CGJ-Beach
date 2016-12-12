#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <SOIL.h>
#include "Shader.h"
#include <vector>

namespace engine {

	class SkyboxTexture
	{
		GLuint texture;

	public:
		void Create(vector<const GLchar*> faces);

		void Destroy();

		void Use() {
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		}
		void UnUse() {
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	};

}