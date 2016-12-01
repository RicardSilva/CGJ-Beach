#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "SOIL.h"
#include "Shader.h"

namespace engine {

	class Texture
	{
		GLuint texture;
		GLuint shaderProgram;

	public:
		void setShader(Shader* shader);
		void create();
		void destroyTextures();
	};

}