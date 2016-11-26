#include "MatrixFactory.h"

Matrix3x3 MatrixFactory::dual3(Vector3 vec) {
	return Matrix3x3(0, -vec.getZ(), vec.getY(),
					vec.getZ(), 0, -vec.getX(),
					-vec.getY(), vec.getX(), 0);
}

Matrix3x3 MatrixFactory::dualsq3(Vector3 vec) {
	return Matrix3x3(-vec.getZ()*vec.getZ() - vec.getY()*vec.getY(), vec.getX()*vec.getY(), vec.getX()*vec.getZ(),
					vec.getX()*vec.getY(), -vec.getZ()*vec.getZ() - vec.getX()*vec.getX(), vec.getY()*vec.getZ(),
					vec.getX()*vec.getZ(), vec.getY()*vec.getZ(), -vec.getY()*vec.getY() - vec.getX()*vec.getX());
}


Matrix4x4 MatrixFactory::create4from3(Matrix3x3 mat) {
	return Matrix4x4(mat.getByIndex(0), mat.getByIndex(1), mat.getByIndex(2), 0,
					mat.getByIndex(3), mat.getByIndex(4), mat.getByIndex(5), 0,
					mat.getByIndex(6), mat.getByIndex(7), mat.getByIndex(8), 0,
					0, 0, 0, 1);
}

Matrix3x3 MatrixFactory::create3from4(Matrix4x4 mat) {
	return Matrix3x3(mat.getByIndex(0), mat.getByIndex(1), mat.getByIndex(2),
					mat.getByIndex(4), mat.getByIndex(5), mat.getByIndex(6),
					mat.getByIndex(8), mat.getByIndex(9), mat.getByIndex(10));
}

Matrix4x4 MatrixFactory::create4fromGL(const Matrix mat)
{
	return Matrix4x4(mat[0], mat[4], mat[8], mat[12],
					mat[1], mat[5], mat[9], mat[13],
					mat[2], mat[6], mat[10], mat[14],
					mat[3], mat[7], mat[11], mat[15]);
}

const GLfloat* MatrixFactory::createGLfrom4(Matrix4x4 mat)
{
	GLfloat m[16] = { mat.getByIndex(0), mat.getByIndex(4),mat.getByIndex(8), mat.getByIndex(12),
					mat.getByIndex(1), mat.getByIndex(5), mat.getByIndex(9), mat.getByIndex(13),
					mat.getByIndex(2), mat.getByIndex(6), mat.getByIndex(10), mat.getByIndex(14),
					mat.getByIndex(3), mat.getByIndex(7), mat.getByIndex(11), mat.getByIndex(15)};
	return m;
}

Matrix4x4 MatrixFactory::createScale4(Vector3 scale) {
	return Matrix4x4(scale.getX(), 0, 0, 0,
					0, scale.getY(), 0, 0,
					0, 0, scale.getZ(), 0,
					0, 0, 0, 1);
}

Matrix4x4 MatrixFactory::createScale4(float x, float y, float z) {
	return Matrix4x4(x, 0, 0, 0,
					0, y, 0, 0,
					0, 0, z, 0,
					0, 0, 0, 1);
}

Matrix4x4 MatrixFactory::createTranslation4(Vector3 trans) {
	return Matrix4x4(1, 0, 0, trans.getX(),
					0, 1, 0, trans.getY(),
					0, 0, 1, trans.getZ(),
					0, 0, 0, 1);
}

Matrix4x4 MatrixFactory::createTranslation4(float x, float y, float z) {
	return Matrix4x4(1, 0, 0, x,
					0, 1, 0, y,
					0, 0, 1, z,
					0, 0, 0, 1);
}

Matrix4x4 MatrixFactory::create4fromQuat(float x, float y, float z, float t) {
	float x2 = x + x;
	float y2 = y + y;
	float z2 = z + z;
	float xx = x * x2;
	float yx = y * x2;
	float yy = y * y2;
	float zx = z * x2;
	float zy = z * y2;
	float zz = z * z2;
	float tx = t * x2;
	float ty = t * y2;
	float tz = t * z2;
	
	Matrix4x4 mat = Matrix4x4(1 - yy - zz, yx + tz, zx - ty, 0,
			yx - tz, 1 - xx - zz, zy + tx, 0,
			zx + ty, zy - tx, 1 - xx - yy,
			0, 0, 0, 0, 1);
	mat.clean();
	return mat;
}

Matrix4x4 MatrixFactory::createViewMatrix(Vector3 eye, Vector3 center, Vector3 up) {
	Vector3 V = center - eye;
	V = V.normalize();
	Vector3 S = V.crossProduct(up);
	S = S.normalize();
	Vector3 U = S.crossProduct(V);
	return Matrix4x4(S.getX(), S.getY(), S.getZ(), -S.dotProduct(eye),
					U.getX(), U.getY(), U.getZ(), -U.dotProduct(eye), 
					-V.getX(), -V.getY(), -V.getZ(), V.dotProduct(eye), 
					0, 0, 0, 1);
}

Matrix4x4 MatrixFactory::createOrthoProj(float l, float r, float b, float t, float n, float f) {
	return Matrix4x4(2 / (r - l), 0, 0, (l + r) / (l - r),
					0, 2 / (t - b), 0, (b + t) / (b - t), 
					0, 0, -2 / (f - n), (n + f) / (n - f),
					0, 0, 0, 1);

}

Matrix4x4 MatrixFactory::createPerspProj(float fovy, float aspect, float nz, float fz) {
	float d = 1 / tan(fovy * PI / 180);
	return Matrix4x4(d/aspect, 0, 0, 0,
					0, d, 0, 0,
					0, 0, (fz + nz) / (nz - fz), (2 * fz * nz) / (nz - fz),
					0, 0, -1, 0);
}


