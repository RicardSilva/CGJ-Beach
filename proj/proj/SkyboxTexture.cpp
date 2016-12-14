#include "SkyboxTexture.h"
#include <iostream>

namespace engine {

	bool isOpenGLError2() {
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

	void checkOpenGLError2(std::string error)
	{
		if (isOpenGLError2()) {
			std::cerr << error << std::endl;
			getchar();
			exit(EXIT_FAILURE);
		}
	}

	void SkyboxTexture::Create(vector<const GLchar*> faces)
	{
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);

		int width, height;
		unsigned char* image;

		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		for (GLuint i = 0; i < faces.size(); i++)
		{
			image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
			);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		checkOpenGLError2("ERROR: Could not create skybox texture.");
	}


	/*void SkyboxTexture::Destroy()
	{
		glDeleteTextures(1, &texture);
	}*/

}