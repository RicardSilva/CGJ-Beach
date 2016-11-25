#pragma once
#include <GL/glew.h>
#include <map>
#include <string>

using namespace std;

namespace engine {

	class Shader
	{

	public:
		Shader(void);
		~Shader(void);
		void LoadFromString(GLenum whichShader, const string& source);
		void LoadFromFile(GLenum whichShader, const string& filename);
		GLuint CreateProgram();
		void LinkProgram();
		void Use();
		void UnUse();

		void BindUniformLocation(GLint location, const GLchar* source);
		void BindAttributeLocation(GLint location, const GLchar* source);
		void BindUniformBlock(GLuint uniformBlockIndex, GLuint uniformBlockBinding);

		GLint GetUniformLocation(const GLchar* source);
		GLint GetAttributeLocation(const GLchar* source);
		GLint GetUniformBlockIndex(const GLchar* source);

		GLuint GetProgram() const;

		void DeleteProgram();
	private:
		enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER };
		GLuint	program;
		int totalShaders;
		GLuint shaders[3];//0->vertexshader, 1->fragmentshader, 2->geometryshader

	};
}
