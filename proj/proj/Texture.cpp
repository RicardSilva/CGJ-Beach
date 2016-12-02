#include "Texture.h"
#include <iostream>

namespace engine {

	bool isOpenGLError() {
		bool isError = false;
		GLenum errCode;
		const GLubyte *errString;
		while ((errCode = glGetError()) != GL_NO_ERROR) {
			isError = true;
			errString = gluErrorString(errCode);
			std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
		}
		return isError;
	}

	void checkOpenGLError(std::string error)
	{
		if (isOpenGLError()) {
			std::cerr << error << std::endl;
			getchar();
			exit(EXIT_FAILURE);
		}
	}

	void Texture::Create()
	{
		glGenTextures(1, &texture);
		int width, height;
		unsigned char* image;

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, texture);
		image = SOIL_load_image("sample.png", &width, &height, 0, SOIL_LOAD_RGB);

		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);

	}


	void Texture::Destroy()
	{
		glDeleteTextures(1, &texture);
	}

}