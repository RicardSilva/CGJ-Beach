#pragma once
#include "Mesh.h"
#include <map>

using namespace std;
namespace engine {

	class MeshManager {

	private:
		static MeshManager *_instance;
		map<string, Mesh*> map;


	public:

		void addMesh(const string& name, Mesh *mesh) {
			map.insert(make_pair(name, mesh));
		}

		Mesh *getMesh(const string& name) {
			return map[name];
		}

		static MeshManager *instance() {
			if (_instance == nullptr)
				_instance = new MeshManager();
			return _instance;
		}


	};
}