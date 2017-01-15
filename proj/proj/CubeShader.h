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

	class CubeShader : public Shader
	{

	public:
		CubeShader(const std::string& vertexShader, const std::string& fragmentShader) : Shader(vertexShader, fragmentShader) {
			BindAttributes();
			GetUniformLocations();
		}
		virtual ~CubeShader() {}

		void BindAttributes(void) {
			BindAttribute(VERTICES, "inPosition");
			BindAttribute(TEXCOORDS, "inTexcoord");
			BindAttribute(NORMALS, "inNormal");
		}


		void GetUniformLocations(void) {
			modelID = GetUniformLocation("ModelMatrix");
			colorID = GetUniformLocation("Color");
			textureID = GetUniformLocation("Texture");
			clipPlaneID = GetUniformLocation("ClipingPlane");
		}


		void LoadModelMatrix(mat4 &m) {
			Shader::LoadMat4(modelID, m);
		}
		void LoadColor(vec3 &v) {
			Shader::LoadVec3(colorID, v);
		}
		void LoadTexture(Texture *t) {
			Shader::LoadTexture(textureID, t);
		}
		void LoadClipingPlane(vec4 &v) {
			Shader::LoadVec4(clipPlaneID, v);
		}

	private:
		GLint modelID, colorID, textureID, clipPlaneID;

	};
}
#pragma once
