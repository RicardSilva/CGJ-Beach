#pragma once
#include "Shader.h"
#include <map>

using namespace std;

namespace engine {

	class ShaderManager {

	public:
		static ShaderManager *_instance;
		map<string, Shader*> map;

		ShaderManager() {}

		void addShader(const string& name, Shader *shader) {
			map.insert(make_pair(name, shader));
		}

		Shader *getShader(const string& name) {
			return map[name];
		}

		static ShaderManager* instance() {
			if (_instance == nullptr)
				_instance = new ShaderManager();
			return _instance;
		}


	};
}