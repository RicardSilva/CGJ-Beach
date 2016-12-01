#include "Shader.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace engine {

	Shader::Shader()
	{
		totalShaders = 0;
		shaders[VERTEX_SHADER] = 0;
		shaders[FRAGMENT_SHADER] = 0;
		shaders[GEOMETRY_SHADER] = 0;

	}

	Shader::~Shader()
	{

	}

	void Shader::LoadFromString(GLenum type, const string& source) {
		GLuint shader = glCreateShader(type);

		const char * ptmp = source.c_str();
		glShaderSource(shader, 1, &ptmp, NULL);

		//check whether the shader loads fine
		GLint status;
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar *infoLog = new GLchar[infoLogLength];
			glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
			cerr << "Compile log: " << infoLog << endl;
			delete[] infoLog;
			glDeleteShader(shader);
		}
		shaders[totalShaders++] = shader;
	}

	GLuint Shader::CreateProgram() {
		program = glCreateProgram();
		if (shaders[VERTEX_SHADER] != 0) {
			glAttachShader(program, shaders[VERTEX_SHADER]);
		}
		if (shaders[FRAGMENT_SHADER] != 0) {
			glAttachShader(program, shaders[FRAGMENT_SHADER]);
		}
		if (shaders[GEOMETRY_SHADER] != 0) {
			glAttachShader(program, shaders[GEOMETRY_SHADER]);
		}

		return program;
	}

	void Shader::LinkProgram() {
		//link and check whether the program links fine
		GLint status;
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			GLint infoLogLength;

			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar *infoLog = new GLchar[infoLogLength];
			glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
			cerr << "Link log: " << infoLog << endl;
			delete[] infoLog;

			glDeleteProgram(program);
			glDeleteShader(shaders[VERTEX_SHADER]);
			glDeleteShader(shaders[FRAGMENT_SHADER]);
			glDeleteShader(shaders[GEOMETRY_SHADER]);

		}

	}

	void Shader::Use() {
		glUseProgram(program);
	}

	void Shader::UnUse() {
		glUseProgram(0);
	}

	void Shader::BindUniformLocation(GLint location, const GLchar* source) {

	}
	void Shader::BindAttributeLocation(GLint location, const GLchar* source) {
		glBindAttribLocation(program, location, source);
	}
	void Shader::BindUniformBlock(GLuint uniformBlockIndex, GLuint uniformBlockBinding) {
		glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
	}

	GLint Shader::GetUniformLocation(const GLchar* source) {
		return glGetUniformLocation(program, source);
	}
	GLint Shader::GetAttributeLocation(const GLchar* source) {
		return glGetAttribLocation(program, source);
	}
	GLint Shader::GetUniformBlockIndex(const GLchar* source) {
		return glGetUniformBlockIndex(program, source);
	}


	void Shader::DeleteProgram() {

		glDetachShader(program, shaders[VERTEX_SHADER]);
		glDetachShader(program, shaders[FRAGMENT_SHADER]);
		//glDetachShader(program, shaders[GEOMETRY_SHADER]);

		glDeleteShader(shaders[VERTEX_SHADER]);
		glDeleteShader(shaders[FRAGMENT_SHADER]);
		//glDeleteShader(shaders[GEOMETRY_SHADER]);

		glDeleteProgram(program); program = -1;
	}


	GLuint Shader::GetProgram() const {
		return program;
	}



	void Shader::LoadFromFile(GLenum whichShader, const string& filename) {
		ifstream fp;
		fp.open(filename.c_str(), ios_base::in);
		if (fp) {

			string buffer(std::istreambuf_iterator<char>(fp), (std::istreambuf_iterator<char>()));
			//copy to source
			LoadFromString(whichShader, buffer);
		}
		else {
			cerr << "Error loading shader: " << filename << endl;
		}
	}
}
