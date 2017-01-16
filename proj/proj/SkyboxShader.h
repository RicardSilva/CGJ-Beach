#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include <map>
#include <string>


#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

using namespace std;

namespace engine {

	class SkyboxShader : public Shader
	{

	public:
		SkyboxShader(const std::string& vertexShader, const std::string& fragmentShader) : Shader(vertexShader, fragmentShader) {
			BindAttributes();
			GetUniformLocations();
		}
		virtual ~SkyboxShader() {}

		void BindAttributes(void) {
			BindAttribute(VERTICES, "inPosition");
			BindAttribute(TEXCOORDS, "inTexcoord");
			BindAttribute(NORMALS, "inNormal");
		}


		void GetUniformLocations(void) {
			modelID = GetUniformLocation("ModelMatrix");
			cubeTextureID = GetUniformLocation("Skybox");

		}


		void LoadModelMatrix(mat4 &m) { Shader::LoadMat4(modelID, m); }
		void LoadCubeTexture() { Shader::LoadInt(cubeTextureID, 0); }

	private:
		GLint modelID, cubeTextureID;

	};
}
