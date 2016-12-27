#pragma once
#include "engine.h"

using namespace std;

namespace engine {

	class WaterSceneNode : public SceneNode {
	private:
		GLuint reflectionTexture;
		GLuint refractionTexture;

	public:

		WaterSceneNode() {}
		virtual ~WaterSceneNode() {}
		void setReflectionTexture(GLuint t) { reflectionTexture = t; }
		void setRefractionTexture(GLuint t) { refractionTexture = t; }

		virtual void draw(mat4 &modelMatrix) {
			//draw this
			if (mesh != nullptr) {
				shader->Use();
				shader->BindTextureUnits();
				shader->LoadModelMatrix(modelMatrix * matrix);
				shader->LoadColor(color);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, reflectionTexture);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, refractionTexture);

				mesh->draw();


				shader->UnUse();

			}


			//draw children
			for (auto node : nodes)
				node->draw(modelMatrix * matrix);
		}

	};

}