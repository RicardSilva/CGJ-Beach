#include "MaterialManager.h"
#include <stddef.h>  // defines NULL

using namespace std;
namespace engine {

	MaterialManager* MaterialManager::_instance = NULL;

	void MaterialManager::AddMaterial(const string& name, Material *material) {
		map.insert(make_pair(name, material));
	}

	Material *MaterialManager::GetMaterial(const string& name) {
		return map[name];
	}

	MaterialManager *MaterialManager::Instance() {
		if (_instance == nullptr)
			_instance = new MaterialManager;
		return _instance;
	}

	void MaterialManager::Destroy() {
		for (auto element : map) {
			delete(element.second);
		}
	}



}