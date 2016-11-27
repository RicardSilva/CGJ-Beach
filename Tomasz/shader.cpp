#include "stdafx.h"
#include "shader.h"
#include "error.h"

using namespace error;

GLuint shader::VertexShaderId = 0;
GLuint shader::FragmentShaderId = 0;
GLuint shader::ProgramId = 0;
GLint shader::UniformModelMatrixId = 0;
GLint shader::UboId = 0;

GLint shader::UniformColorId = 0;
const GLuint shader::UBO_BP = 0;

char * readFile(char * name) {
	std::ifstream t;
	int length;
	char * text;
	t.open(name);      
	t.seekg(0, std::ios::end);  
	length = t.tellg();           
	t.seekg(0, std::ios::beg);   
	text = new char[length];   
	t.read(text, length); 
	t.close();
	return text;
}

// loads shaders from file, reads two uniforms: vec4 "Color" and mat4 "Matrix" 
 void shader::createShaderProgramFromFiles(GLchar * vertex_name, GLchar * fragment_name) {

	char * vertex=readFile(vertex_name);
	char * fragment = readFile(fragment_name);
	checkOpenGLError("ERROR: Could not create shaders.1");

	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &vertex, 0);
	glCompileShader(VertexShaderId);
	checkOpenGLError("ERROR: Could not create shaders.2");

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &fragment, 0);
	glCompileShader(FragmentShaderId);
	checkOpenGLError("ERROR: Could not create shaders.3");

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);
	checkOpenGLError("ERROR: Could not create shaders.4");

	glBindAttribLocation(ProgramId, 0, "in_Position");
	glLinkProgram(ProgramId);
	UniformModelMatrixId = glGetUniformLocation(ProgramId, "ModelMatrix");
	UniformColorId = glGetUniformLocation(ProgramId, "Color");
	UboId = glGetUniformBlockIndex(ProgramId, "SharedMatrices");
	glUniformBlockBinding(ProgramId, UboId, UBO_BP);

	checkOpenGLError("ERROR: Could not create shaders.5");
}

void shader::destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}