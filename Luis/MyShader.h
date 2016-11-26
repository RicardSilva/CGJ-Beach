#ifndef MyShader_H
#define MyShader_H

#include "Matrix4x4.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

bool isOpenGLError();
void checkOpenGLError(std::string error);

class MyShader {
public:
	GLuint VertexShaderId, FragmentShaderId, ProgramId;
	GLint ModelMatrix_UId, ViewMatrix_UId, ProjectionMatrix_UId;
	const GLuint UBO_BP = 0;

	std::vector <Vector3> Vertices;
	std::vector <Vector2> Texcoords;
	std::vector <Vector3> Normals;

	bool TexcoordsLoaded, NormalsLoaded;

	MyShader() {}

	std::string read(const std::string& filename);

	/////////////////////////////////////////////////////////////////////// SHADERs

	const GLuint checkCompilation(const GLuint shader_id, const std::string& filename)
	{
		GLint compiled;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
		if (compiled == GL_FALSE) {
			GLint length;
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
			GLchar* const log = new char[length];
			glGetShaderInfoLog(shader_id, length, &length, log);
			std::cerr << "[" << filename << "] " << std::endl << log;
			delete log;
		}
		return compiled;
	}

	void checkLinkage(const GLuint program_id);

	const GLuint addShader(const GLuint program_id, const GLenum shader_type, const std::string& filename);

	void createShaderProgram(std::string& vs_file, std::string& fs_file);

	void destroyShaderProgram();

	void changeDomColor(Vector4 color);
};


#endif