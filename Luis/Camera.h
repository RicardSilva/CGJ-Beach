#ifndef Camera_H
#define Camera_H

#include "MatrixFactory.h"

class Camera {
public:
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjectionMatrix;
	MatrixFactory Factory;

	Camera(){}
	Camera(MatrixFactory mf, Vector3 eye, Vector3 center, Vector3 up, float l, float r, float b, float t, float n, float f) {
		ViewMatrix = mf.createViewMatrix(eye, center, up);
		ProjectionMatrix = mf.createOrthoProj(l, r, b, t, n, f);
	}
	
	Camera(MatrixFactory mf, Vector3 eye, Vector3 center, Vector3 up, float fovy, float aspect, float nz, float fz) {
		ViewMatrix = mf.createViewMatrix(eye, center, up);
		ProjectionMatrix = mf.createPerspProj(fovy, aspect, nz, fz);
	}
	
	void activateViewMatrix(GLint view_uid){
		glUniformMatrix4fv(view_uid, 1, GL_FALSE, Factory.createGLfrom4(ViewMatrix));
	}
	
	void activateProjMatrix(GLint proj_uid){
		glUniformMatrix4fv(proj_uid, 1, GL_FALSE, Factory.createGLfrom4(ProjectionMatrix));
	}
};


#endif