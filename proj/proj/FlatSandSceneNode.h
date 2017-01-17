#pragma once
#include "engine.h"

using namespace std;

namespace engine {

	class FlatSandSceneNode : public SceneNode {


	private:
		GLuint Texture;
		GLuint Ks;
		GLuint Kd;
		GLuint N;

	public:

		FlatSandSceneNode() {
		}

		virtual ~FlatSandSceneNode() {}

		void setShader(Shader* s) {
			shader = s;
			shader->Use();
			Ks = s->GetUniformLocation("Ks");
			Kd = s->GetUniformLocation("Kd");
			N = s->GetUniformLocation("Ns");
			shader->BindTextureUnits();
			shader->LoadColor(vec3(0.7255f, 0.8745f, 1.0f));
			shader->UnUse();

		}
		void setTexture(GLuint t) { Texture = t; }

		virtual void draw(mat4 &modelMatrix) {

			shader->Use();
			shader->LoadModelMatrix(modelMatrix * matrix);

			glCullFace(GL_FRONT);
			mesh->draw();
			glCullFace(GL_BACK);
			shader->UnUse();



			//draw children
			for (auto node : nodes)
				node->draw(modelMatrix * matrix);
		}

	};
}
