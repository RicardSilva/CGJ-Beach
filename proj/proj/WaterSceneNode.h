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
		GLuint depthTexture;
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

		void setShader(Shader* s) {
			shader = s;
			shader->Use();
			shader->BindTextureUnits();

			shader->LoadColor(color);
			shader->UnUse();

		}
		void setReflectionTexture(GLuint t) { reflectionTexture = t; }
		void setRefractionTexture(GLuint t) { refractionTexture = t; }
		void setDepthTexture(GLuint t) { depthTexture = t; }

		virtual void draw(mat4 &modelMatrix) {
			movementFactor += movementSpeed;
			if (movementFactor >= 1) movementFactor -= 1;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			shader->Use();
			shader->LoadMovementFactor(movementFactor);
			shader->LoadModelMatrix(modelMatrix * matrix);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, reflectionTexture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, refractionTexture);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, dudvTexture);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, normalTexture);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, depthTexture);

			mesh->draw();


			shader->UnUse();
			glDisable(GL_BLEND);
			


			//draw children
			for (auto node : nodes)
				node->draw(modelMatrix * matrix);
		}

	};

}