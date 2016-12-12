#pragma once
#include "Shader.h"
#include <map>


namespace engine {

	class ShaderManager {

	private:
		static ShaderManager *_instance;
		map<string, Shader*> map;
		ShaderManager() {};
		ShaderManager(ShaderManager const&) {};
		ShaderManager& operator=(ShaderManager const&) {};


	public:

		void AddShader(const string& name, Shader *shader);
		Shader *GetShader(const string& name);

		static ShaderManager *Instance();
		void Destroy();

	};
}