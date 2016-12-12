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
		
		bool isOpenGLError() {
			bool isError = false;
			GLenum errCode;
			const GLubyte *errString;
			while ((errCode = glGetError()) != GL_NO_ERROR) {
				isError = true;
				errString = gluErrorString(errCode);
				std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
			}
			return isError;
		}

		void checkOpenGLError(std::string error)
		{
			if (isOpenGLError()) {
				std::cerr << error << std::endl;
				getchar();
				exit(EXIT_FAILURE);
			}
		}

		void draw() {

			// View and projection matrix loaded to UBO
			shader->Use();
			GLuint UboID = shader->GetUniformBlockIndex("SharedMatrices");
			GLint blockSize;
			glGetActiveUniformBlockiv(shader->GetProgram(), UboID,
				GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
			GLubyte * blockBuffer = (GLubyte *)malloc(blockSize);
			const GLchar *names[] = { "ViewMatrix", "ProjectionMatrix" };
			GLuint indices[2];
			glGetUniformIndices(shader->GetProgram(), 2, names, indices);
			GLint offset[2];
			glGetActiveUniformsiv(shader->GetProgram(), 2, indices, GL_UNIFORM_OFFSET, offset);
			memcpy(blockBuffer + offset[0], camera->getViewMatrix().Transposed().Export(),
				sizeof(mat4));
			memcpy(blockBuffer + offset[1], camera->getProjMatrix().Transposed().Export(),
				sizeof(mat4));
			GLuint uboHandle;
			glGenBuffers(1, &uboHandle);
			glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
			glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer,
				GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, UboID, uboHandle);
			shader->UnUse();

			this->getRoot()->draw(matFactory::Identity4());
		}

	};



}