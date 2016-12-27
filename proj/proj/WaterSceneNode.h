#pragma once
#include "engine.h"

using namespace std;

const float movementSpeed = 0.00005f;

namespace engine {

	class WaterSceneNode : public SceneNode {

		
	private:
		GLuint reflectionTexture;
		GLuint refractionTexture;
		GLuint dudvTexture;
		GLuint normalTexture;
		float movementFactor;
		

	public:

		WaterSceneNode() {
			Texture *dudv = new Texture("dudvmap.png");
			dudvTexture = dudv->TextureID();
			Texture *normal = new Texture("waterNormalMap.png");
			normalTexture = normal->TextureID();

			movementFactor = 0;
		}
		virtual ~WaterSceneNode() {}
		void setReflectionTexture(GLuint t) { reflectionTexture = t; }
		void setRefractionTexture(GLuint t) { refractionTexture = t; }

		virtual void draw(mat4 &modelMatrix) {
			movementFactor += movementSpeed;
			if (movementFactor >= 1) movementFactor -= 1;
			shader->Use();
			shader->LoadMovementFactor(movementFactor);
			//draw this
			shader->BindTextureUnits();
			shader->LoadModelMatrix(modelMatrix * matrix);
			shader->LoadColor(color);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, reflectionTexture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, refractionTexture);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, dudvTexture);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, normalTexture);

			mesh->draw();


				shader->UnUse();

			


			//draw children
			for (auto node : nodes)
				node->draw(modelMatrix * matrix);
		}

	};

}