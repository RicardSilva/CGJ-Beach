#include "ShaderManager.h"
#include <stddef.h>  // defines NULL
using namespace std;
namespace engine {

	ShaderManager* ShaderManager::_instance = NULL;

	void ShaderManager::AddShader(const string& name, Shader *shader) {
		map.insert(make_pair(name, shader));
	}

	Shader *ShaderManager::GetShader(const string& name) {
		return map[name];
	}

	ShaderManager *ShaderManager::Instance() {
		if (_instance == nullptr)
			_instance = new ShaderManager;
		return _instance;
	}

	void ShaderManager::Destroy() {
		for (auto element : map) {
			delete(element.second);
		}
		delete(_instance);
	}



}