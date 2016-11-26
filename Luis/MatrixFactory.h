#ifndef MatrixFactory_H
#define MatrixFactory_H

#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Vector3.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

typedef GLfloat Matrix[16];

#define PI 3.14159265

class MatrixFactory {
public:
	MatrixFactory(){}

	Matrix3x3 zeroMat3() { return Matrix3x3(); }
	Matrix3x3 identity3() { return Matrix3x3(1, 0, 0, 0, 1, 0, 0, 0, 1); }
	Matrix3x3 dual3(Vector3 vec);
	Matrix3x3 dualsq3(Vector3 vec);


	Matrix4x4 zeroMat4() { return Matrix4x4(); }
	Matrix4x4 identity4() { return create4from3(identity3()); }
	Matrix4x4 dual4(Vector3 vec) { return create4from3(dual3(vec)); }
	Matrix4x4 dualsq4(Vector3 vec) { return create4from3(dualsq3(vec)); }

	Matrix4x4 create4from3(Matrix3x3 mat);
	Matrix3x3 create3from4(Matrix4x4 mat);
	Matrix4x4 create4fromGL(const Matrix mat);
	const GLfloat* createGLfrom4(Matrix4x4 mat);
	
	Matrix4x4 createScale4(Vector3 scale);
	Matrix4x4 createScale4(float x, float y, float z);
	
	Matrix4x4 createTranslation4(Vector3 trans);
	Matrix4x4 createTranslation4(float x, float y, float z);

	Matrix4x4 create4fromQuat(float x, float y, float z, float w);

	Matrix4x4 createViewMatrix(Vector3 eye, Vector3 center, Vector3 up);
	Matrix4x4 createOrthoProj(float l, float r, float b, float t, float n, float f);
	Matrix4x4 createPerspProj(float fovy, float aspect, float nz, float fz);


	Matrix4x4 createRotation(Vector3 axis, float angle) {
		Vector3 vec = axis.normalize();
		float s = sin(angle * PI / 180);
		float c = 1 - cos(angle * PI / 180);
		Matrix3x3 rot = identity3() + s * dual3(vec) + c * dualsq3(vec);
		return create4from3(rot);
	}
};


#endif