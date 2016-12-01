#include "Texture.h"


namespace engine {

	void Texture::setShader(Shader* shader) {
		shaderProgram = shader->GetProgram();
	}

	void Texture::create()
	{
		// Load textures
		glGenTextures(1, &texture);

		int width, height;
		unsigned char* image;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		image = SOIL_load_image("sample.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}


	void Texture::destroyTextures()
	{
		glDeleteTextures(1, &texture);
	}

}