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

	class WaterShader : public Shader
	{

	public:
		WaterShader() : Shader() {}
		WaterShader(const std::string& vertexShader, const std::string& fragmentShader) : Shader(vertexShader, fragmentShader) {}
		virtual ~WaterShader() {}
		
		void BindAttributes(void) {
			BindAttribute(VERTICES, "inPosition");
			BindAttribute(TEXCOORDS, "inTexcoord");
			BindAttribute(NORMALS, "inNormal");
		}


		void GetUniformLocations(void) {
			modelID = GetUniformLocation("ModelMatrix");
			colorID = GetUniformLocation("Color");
			reflectionID = GetUniformLocation("ReflectionTexture");
			refractionID = GetUniformLocation("RefractionTexture");
			dudvMapID = GetUniformLocation("dudvMap");
			movementID = GetUniformLocation("movementFactor");
			normalMapID = GetUniformLocation("normalMap");
			lightPositionID = GetUniformLocation("lightPosition");
			lightColorID = GetUniformLocation("intensities");
			depthMapID = GetUniformLocation("depthMap");

		}

		void BindTextureUnits() {
			Shader::LoadInt(reflectionID, 0);
			Shader::LoadInt(refractionID, 1);
			Shader::LoadInt(dudvMapID, 2);
			Shader::LoadInt(normalMapID, 3);
			Shader::LoadInt(depthMapID, 4);
		}

		void LoadModelMatrix(mat4 &m) {	Shader::LoadMat4(modelID, m);}
		void LoadColor(vec3 &v) { Shader::LoadVec3(colorID, v);	}
		void LoadMovementFactor(float f) { Shader::LoadFloat(movementID, f); }
		
	private:
		GLint modelID, colorID, reflectionID, refractionID, dudvMapID, normalMapID, depthMapID, movementID, lightPositionID, lightColorID;

	};
}
