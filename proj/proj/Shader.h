#pragma once
#include <GL/glew.h>
#include <map>
#include "vec.h"
#include "mat.h"
#include "Texture.h"
#include <string>

using namespace std;

namespace engine {

	class Shader
	{

	public:
		Shader(void);
		Shader(const std::string& vertexShader, const std::string& fragmentShader);
		void Init(const std::string& vertexShader, const std::string& fragmentShader);
		virtual ~Shader(void);
		void LoadFromString(GLenum whichShader, const std::string& source);
		void LoadFromFile(GLenum whichShader, const std::string& filename);
		GLuint CreateProgram();
		void LinkProgram();

		GLuint GetProgram() const {
			return program;
		}

		void Use();
		void UnUse();

		virtual void BindAttributes(void) = 0;
		virtual void GetUniformLocations(void) = 0;


		virtual void LoadModelMatrix(mat4 &m) {};
		virtual void LoadColor(vec3 &v) {};
		//virtual void LoadTexture(Texture *t) {};

		void LoadFloat(GLint location, GLfloat f);
		void LoadVec3(GLint location, vec3 &v);
		void LoadMat4(GLint location, mat4 &m);
		//void LoadTexture(GLint location, Texture *t);

		void BindAttribute(GLint attribute, const GLchar* variableName);
		GLint GetUniformLocation(const GLchar* source);
		//void BindUniformBlock(GLuint uniformBlockIndex, GLuint uniformBlockBinding);
		GLint GetUniformBlockIndex(const GLchar* source);


		void DeleteProgram();

	private:
		enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER};		
		int totalShaders;
		GLuint shaders[2];//0->vertexshader, 1->fragmentshaders
	protected:
		GLuint	program;
		GLint modelID, colorID;
	};
}
