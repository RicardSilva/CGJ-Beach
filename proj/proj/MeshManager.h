#pragma once
#include "Mesh.h"
#include <map>


namespace engine {

	class MeshManager {

	private:
		static MeshManager *_instance;
		map<string, Mesh*> map;
		MeshManager() {};
		MeshManager(MeshManager const&) {};
		MeshManager& operator=(MeshManager const&) {};


	public:

		void AddMesh(const string& name, Mesh *mesh);
		Mesh *GetMesh(const string& name);

		static MeshManager *Instance();
		void Destroy();


	};
}