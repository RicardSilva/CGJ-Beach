#pragma once

#include "Texture.h"
#include <vector>
#include <string>

namespace engine {

	class SkyboxTexture : public Texture
	{

	public:
		SkyboxTexture(std::vector<const GLchar*> faces) {
			Create(faces);
		}
		SkyboxTexture(GLuint t) : Texture(t) {}


		void Use() {
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		}
		void UnUse() {
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	private:
		void Create(std::vector<const GLchar*> faces);
	};

}