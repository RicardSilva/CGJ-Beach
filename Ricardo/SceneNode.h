#pragma once
#include "engine.h"
#include <vector>

using namespace std;

namespace engine {

	class SceneNode {
	private:
		Mesh *mesh;
		mat4 matrix;
		vec3 color;
		Shader *shader;
		vector<SceneNode*> nodes;

		GLint viewId, projID, modelID, colorID;

	public:

		SceneNode() {

		}

		void setMatrix(mat4 &m) { matrix = m; }
		mat4 getMatrix() { return matrix; }

		void setMesh(Mesh* m) { mesh = m; }
		Mesh *getMesh() { return mesh; }

		void setColor(vec3& v) { color = v; }

		void setShader(Shader* s) { 
			shader = s; 
			viewId = s->GetUniformLocation("ViewMatrix");
			projID = s->GetUniformLocation("ProjectionMatrix");
			modelID = s->GetUniformLocation("ModelMatrix");
			colorID = s->GetUniformLocation("Color");
		}
		Shader *getShader() { return shader; }

		void draw(mat4 &viewMatrix, mat4 &projMatrix, mat4 &modelMatrix) {
			//draw this
			if (mesh != nullptr) {

				shader->Use();
				glUniformMatrix4fv(viewId, 1, GL_FALSE, viewMatrix.Transposed().Export());
				glUniformMatrix4fv(projID, 1, GL_FALSE, projMatrix.Transposed().Export());
				glUniformMatrix4fv(modelID, 1, GL_FALSE, (modelMatrix * matrix).Transposed().Export());
				glUniform3fv(colorID, 1, color.Export());
				
				mesh->draw();

				shader->UnUse();
			}
			//draw children
			for (auto node : nodes)
				node->draw(viewMatrix, projMatrix, modelMatrix * matrix);
		}

		void addNode(SceneNode *node) {
			nodes.push_back(node);
		}
	};

}