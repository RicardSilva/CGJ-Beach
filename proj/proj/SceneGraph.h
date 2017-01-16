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
		GLuint uboHandle;
		GLuint UboID;
		GLubyte *blockBuffer;
		GLint blockSize;
		GLuint indices[7];
		GLint offset[7];
		GLchar *names[7]; 
	
		SceneGraph(Camera *camera, Shader *shader)
		{
			names[0] = "ViewMatrix";
			names[1] = "ProjectionMatrix";
			names[2] = "ClipingPlane";
			names[3] = "lightPosition";
			names[4] = "intensities";
			names[5] = "attenuation";
			names[6] = "ambientCoefficient";
			this->setCamera(camera);
			this->setShader(shader);
			this->InitializeUniformBlock();

		}
		virtual ~SceneGraph() {
			
			delete(root);
			delete(blockBuffer);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void InitializeUniformBlock() {
			uboHandle = 0;
			glGenBuffers(1, &uboHandle);
			glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
			UboID = shader->GetUniformBlockIndex("SharedMatrices");
			glGetActiveUniformBlockiv(shader->GetProgram(), UboID,
				GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
			blockBuffer = (GLubyte *)malloc(blockSize);

			glGetUniformIndices(shader->GetProgram(), 7, names, indices);
			glGetActiveUniformsiv(shader->GetProgram(), 7, indices, GL_UNIFORM_OFFSET, offset);

			glBindBufferBase(GL_UNIFORM_BUFFER, UboID, uboHandle);

			GLfloat atten = 0.01f;
			GLfloat ambient = 0.2f;
						
			memcpy(blockBuffer + offset[0], camera->getViewMatrix().Transposed().Export(), sizeof(mat4));
			memcpy(blockBuffer + offset[1], camera->getProjMatrix().Transposed().Export(), sizeof(mat4));
			memcpy(blockBuffer + offset[2], camera->getClippingPlane().Export(), sizeof(vec4));
			memcpy(blockBuffer + offset[3], new vec3(50, 10, 50), sizeof(vec3));
			memcpy(blockBuffer + offset[4], new vec3(1, 1, 1), sizeof(vec3));
			memcpy(blockBuffer + offset[5], &atten, sizeof(GLfloat));
			memcpy(blockBuffer + offset[6], &ambient, sizeof(GLfloat));

			glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);

		}

		void setCamera(Camera *c) { camera = c; }
		Camera* getCamera() { return camera; }

		void setShader(Shader *s) { shader = s; }
		Shader* getShader() { return shader; }

		void setRoot(SceneNode *r) { root = r; }
		SceneNode *getRoot() { return root; }

		

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
		
			memcpy(blockBuffer + offset[2], camera->getClippingPlane().Export(), sizeof(vec4));
			memcpy(blockBuffer + offset[0], camera->getViewMatrix().Transposed().Export(),	sizeof(mat4));

			glBufferSubData(GL_UNIFORM_BUFFER, 0, blockSize, blockBuffer);

			this->getRoot()->draw(matFactory::Identity4());
		}

	};



}