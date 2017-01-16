#pragma once
#include "engine.h"

using namespace std;


namespace engine {

	class SkyboxSceneNode : public SceneNode {
		
	public:

		SkyboxSceneNode() {
		}
		virtual ~SkyboxSceneNode() {}

		void setShader(Shader* s) {
			shader = s;
			shader->Use();
			shader->LoadCubeTexture();
			shader->UnUse();
		}

		virtual void draw(mat4 &modelMatrix) {


			glCullFace(GL_FRONT);
			glDisable(GL_DEPTH_TEST);
			shader->Use();
			shader->LoadModelMatrix(modelMatrix * matrix);

			glActiveTexture(GL_TEXTURE0);
			texture->Use();

			mesh->draw();

			shader->UnUse();

			glCullFace(GL_BACK);
			glEnable(GL_DEPTH_TEST);


			//draw children
			for (auto node : nodes)
				node->draw(modelMatrix * matrix);
		}

	};

}