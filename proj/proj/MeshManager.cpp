#include "MeshManager.h"
#include <stddef.h>  // defines NULL
using namespace std;
namespace engine {

MeshManager* MeshManager::_instance = NULL;

void MeshManager::AddMesh(const string& name, Mesh *mesh) {
	map.insert(make_pair(name, mesh));
}

Mesh *MeshManager::GetMesh(const string& name) {
	return map[name];
}

MeshManager *MeshManager::Instance() {
	if (_instance == nullptr)
		_instance = new MeshManager;
	return _instance;
}

void MeshManager::Destroy() {
	for (auto element : map) {
		delete(element.second);
	}
	delete(_instance);
}


	
}