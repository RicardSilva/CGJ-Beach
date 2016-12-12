#pragma once
#include "engine.h"
#include <vector>
#include "Texture.h"
#include "SkyboxTexture.h"


using namespace std;

namespace engine {

	class SceneNode {
	private:
		Mesh *mesh;
		mat4 matrix;
		vec3 color;
		Shader *shader;
		Texture *texture;
		SkyboxTexture *skyboxTexture;
		vector<SceneNode*> nodes;

		GLint viewId, projID, modelID, colorID, textureID, skyboxID;

	public:

		SceneNode() {}
		virtual ~SceneNode() {
			for (auto node : nodes)
				delete(node);
		}
		void setMatrix(mat4 &m) { matrix = m; }
		mat4 getMatrix() { return matrix; }

		void setMesh(Mesh* m) { mesh = m; }
		Mesh *getMesh() { return mesh; }

		void setColor(vec3& v) { color = v; }
		void setTexture(Texture* t) { texture = t; }
		void setSkybox(SkyboxTexture* st) { skyboxTexture = st; }

		void setShader(Shader* s) { 
			shader = s; 
			viewId = s->GetUniformLocation("ViewMatrix");
			projID = s->GetUniformLocation("ProjectionMatrix");
			modelID = s->GetUniformLocation("ModelMatrix");
			colorID = s->GetUniformLocation("Color");
			textureID = s->GetUniformLocation("Texture");

			shader->Use();
			glUniform3fv(colorID, 1, color.Export());
			if (texture != nullptr) {
				texture->Use();
				glUniform1i(textureID, 0);
				texture->UnUse();
			}
			shader->UnUse();
				
		}
		void setShaderSkybox(Shader* s) {
			shader = s;
			viewId = s->GetUniformLocation("ViewMatrix");
			projID = s->GetUniformLocation("ProjectionMatrix");
			modelID = s->GetUniformLocation("ModelMatrix");
			colorID = s->GetUniformLocation("Color");
			textureID = s->GetUniformLocation("Texture");
			skyboxID = s->GetUniformLocation("skybox");
			shader->Use();
			glUniform3fv(colorID, 1, color.Export());
			if (texture != nullptr) {
				texture->Use();
				glUniform1i(textureID, 0);
				texture->UnUse();
			}
			if (skyboxTexture != nullptr) {
				skyboxTexture->Use();
				glUniform1i(skyboxID, 0);
				skyboxTexture->UnUse();
			}
			shader->UnUse();
		}

		Shader *getShader() { return shader; }

		void draw(mat4 &modelMatrix) {
			//draw this
			
			if (mesh != nullptr) {
				shader->Use();

				glUniformMatrix4fv(modelID, 1, GL_FALSE, (modelMatrix * matrix).Transposed().Export());
				
				if (texture != nullptr){
					glCullFace(GL_BACK);
					texture->Use();
				}
				if (skyboxTexture != nullptr) {
					glCullFace(GL_FRONT);
					skyboxTexture->Use();
				}
				mesh->draw();
				skyboxTexture->UnUse();
				texture->UnUse();
				
				shader->UnUse();

			}


			//draw children
			for (auto node : nodes)
				node->draw(modelMatrix * matrix);
		}

		void addNode(SceneNode *node) {
			nodes.push_back(node);
		}
	};

}