//#pragma once
#include <string>

class shader
{
public:
		static const GLchar* VertexShader;
		static const GLchar* FragmentShader;
		static GLuint VertexShaderId, FragmentShaderId, ProgramId;
		static GLint UniformModelMatrixId, UniformColorId, UboId;
		static const GLuint UBO_BP;

		static void createShaderProgramFromFiles(GLchar *, GLchar *);
		static void destroyShaderProgram();

};

