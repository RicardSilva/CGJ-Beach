#pragma once
#include "engine.h"
#include <vector>

using namespace std;

namespace engine {

	class SceneGraph {
	public:
		Camera* camera;
		Shader* shader;
		SceneNode* root;

		//SceneNode *ground, *base, *ST1, *ST2, *MT, *BT1, *BT2, *S, *P;
		SceneNode *cube;

	
		SceneGraph(Camera *camera, Shader *shader)
		{
			this->setCamera(camera);
			this->setShader(shader);
		}

		void setCamera(Camera *c) { camera = c; }
		Camera* getCamera() { return camera; }

		void setShader(Shader *s) { shader = s; }
		Shader* getShader() { return shader; }

		void setRoot(SceneNode *r) { root = r; }
		SceneNode *getRoot() { return root; }


		


		void draw() {
			this->getRoot()->draw(camera->getViewMatrix(), camera->getProjMatrix(), matFactory::Identity4());
		}

	};



}