#pragma once
#include "SceneGraph.h"
#include <map>

using namespace std;

namespace engine {

	class SceneGraphManager {

	public:

		static SceneGraphManager *_instance;
		map<string, SceneGraph*> map;


		void addSceneGraph(const string& name, SceneGraph *sceneGraph) {
			map.insert(make_pair(name, sceneGraph));
		}

		SceneGraph getSceneGraph(const string& name) {
			return *map[name];
		}

		static SceneGraphManager *instance() {
			if (_instance == nullptr)
				_instance = new SceneGraphManager();
			return _instance;
		}


	};
}