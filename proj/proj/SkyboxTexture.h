#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <SOIL.h>
#include "Texture.h"
#include <vector>

namespace engine {

	class SkyboxTexture : public Texture
	{
		//GLuint texture;

	public:
		SkyboxTexture(vector<const GLchar*> faces) {
			Create(faces);
		}

		

		//void Destroy();

		void Use() {
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		}
		void UnUse() {
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	private:
		void Create(vector<const GLchar*> faces);
	};

}