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

		SceneNode *ground, *base, *ST1, *ST2, *MT, *BT1, *BT2, *S, *P;

	
		SceneGraph(Camera *camera, Shader *shader)
		{
			this->setCamera(camera);
			this->setShader(shader);
			createEnvironmentSceneGraph(this);
		}

		void setCamera(Camera *c) { camera = c; }
		Camera* getCamera() { return camera; }

		void setShader(Shader *s) { shader = s; }
		Shader* getShader() { return shader; }

		void setRoot(SceneNode *r) { root = r; }
		SceneNode *getRoot() { return root; }


		void createEnvironmentSceneGraph(SceneGraph* scenegraph)
		{
		
			Mesh* cubeMesh = new Mesh(std::string("cube.obj"));
			Mesh* triangularMesh = new Mesh(std::string("triangular.obj"));
			Mesh* parallelogramMesh = new Mesh(std::string("parallelogram.obj"));


			ground = new SceneNode();
			ground->setMesh(cubeMesh);
			ground->setMatrix(
				 matFactory::Translate3(-2,-2,0) * matFactory::Scale3(4, 4, 1)
			);
			ground->setShader(shader);
			ground->setColor(vec3(0, 0, 0));
			scenegraph->setRoot(ground);
			
			base = new SceneNode();
			base->setMatrix(
				
				matFactory::Translate3(0.5, 0.5, 1) * matFactory::Scale3(0.5, 0.5, 1)
				);
			base->setShader(shader);
			ground->addNode(base);

			
			ST1 = new SceneNode();
			ST1->setMesh(triangularMesh);
			ST1->setShader(shader);
			ST1->setColor(vec3(1, 0, 0));
			base->addNode(ST1);

			
			ST2 = new SceneNode();
			ST2->setMesh(triangularMesh);
			ST2->setShader(shader);
			ST2->setColor(vec3(0,1,0));
			base->addNode(ST2);

			
			MT = new SceneNode();
			MT->setMesh(triangularMesh);
			MT->setShader(shader);
			MT->setColor(vec3(0, 0, 1));
			base->addNode(MT);

			BT1 = new SceneNode();
			BT1->setMesh(triangularMesh);
			BT1->setShader(shader);
			BT1->setColor(vec3(1, 1, 0));
			base->addNode(BT1);
			
			BT2 = new SceneNode();
			BT2->setMesh(triangularMesh);
			BT2->setShader(shader);
			BT2->setColor(vec3(1, 0, 1));
			base->addNode(BT2);
			 
			S = new SceneNode();
			S->setMesh(cubeMesh);
			S->setShader(shader);
			S->setColor(vec3(0, 1, 1));
			base->addNode(S);
			
			P = new SceneNode();
			P->setMesh(parallelogramMesh);
			P->setShader(shader);
			P->setColor(vec3(1, 1, 1));
			base->addNode(P);
			
		}


		void draw() {
			this->getRoot()->draw(camera->getViewMatrix(), camera->getProjMatrix(), matFactory::Identity4());
		}

	};



}