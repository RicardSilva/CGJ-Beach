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
		SceneNode* skyBox;


	
		SceneGraph(Camera *camera, Shader *shader)
		{
			this->setCamera(camera);
			this->setShader(shader);
		}
		virtual ~SceneGraph() {
			delete(camera);
			delete(root);
			delete(skyBox);
		}
		void setCamera(Camera *c) { camera = c; }
		Camera* getCamera() { return camera; }

		void setShader(Shader *s) { shader = s; }
		Shader* getShader() { return shader; }

		void setRoot(SceneNode *r) { root = r; }
		SceneNode *getRoot() { return root; }

		void setSkyBox(SceneNode *s) { skyBox = s; }
		SceneNode *getSkyBox() { return skyBox; }


		void draw() {
			this->getRoot()->draw(camera->getViewMatrix(), camera->getProjMatrix(), matFactory::Identity4());
		}

	};



}