#pragma once
#include "engine.h"

using namespace std;

namespace engine {

	class SandSceneNode : public SceneNode {


	private:
		GLuint Texture;
		GLuint Ks;
		GLuint Kd;
		GLuint N;

	public:

		SandSceneNode() {
		}

		virtual ~SandSceneNode() {}

		void setShader(Shader* s) {
			shader = s;
			shader->Use();
			Ks = s->GetUniformLocation("Ks");
			Kd = s->GetUniformLocation("Kd");
			N = s->GetUniformLocation("Ns");
			shader->BindTextureUnits();
			shader->LoadColor(color);
			shader->UnUse();

		}
		void setTexture(GLuint t) { Texture = t; }

		virtual void draw(mat4 &modelMatrix) {
			
			shader->Use();
			shader->LoadModelMatrix(modelMatrix * matrix);

			shader->LoadTexture(texture);
			glActiveTexture(GL_TEXTURE0);
			texture->Use();
			glUniform3fv(Ks, 1, material->getKs().Export());
			glUniform3fv(Kd, 1, material->getKd().Export());
			glUniform1f(N, material->getNs());

			mesh->draw();

			texture->UnUse();
			shader->UnUse();
			glDisable(GL_BLEND);



			//draw children
			for (auto node : nodes)
				node->draw(modelMatrix * matrix);
		}

	};

}