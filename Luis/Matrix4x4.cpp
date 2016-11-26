#include "Matrix4x4.h"

Matrix4x4 Matrix4x4::transpose() {
	return Matrix4x4(_data[0], _data[4], _data[8], _data[12], _data[1], _data[5], _data[9], _data[13], _data[2], _data[6], _data[10], _data[14], _data[3], _data[7], _data[11], _data[15]);
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4 & mat) {
	Matrix4x4 matrix = *this;
	matrix += mat;
	return matrix;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4 & mat) {
	Matrix4x4 matrix = *this;
	matrix -= mat;
	return matrix;
}

Matrix4x4 Matrix4x4::operator*(float num) {
	Matrix4x4 matrix = *this;
	matrix *= num;
	return matrix;
}

Matrix4x4 operator*(float num, const Matrix4x4& mat) {
	Matrix4x4 matrix = mat;
	matrix *= num;
	return matrix;
}

Vector4 Matrix4x4::operator*(const Vector4& vec) {
	float x = _data[0] * vec.getX() + _data[1] * vec.getY() + _data[2] * vec.getZ() + _data[3] * vec.getW();
	float y = _data[4] * vec.getX() + _data[5] * vec.getY() + _data[6] * vec.getZ() + _data[7] * vec.getW();
	float z = _data[8] * vec.getX() + _data[9] * vec.getY() + _data[10] * vec.getZ() + _data[11] * vec.getW();
	float w = _data[12] * vec.getX() + _data[13] * vec.getY() + _data[14] * vec.getZ() + _data[15] * vec.getW();
	return Vector4(x, y, z, w);
}


Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat) {
	Matrix4x4 ret = Matrix4x4();
	ret.setByIndex(0, _data[0] * mat._data[0] + _data[1] * mat._data[4] + _data[2] * mat._data[8] + _data[3] * mat._data[12]);
	ret.setByIndex(1, _data[0] * mat._data[1] + _data[1] * mat._data[5] + _data[2] * mat._data[9] + _data[3] * mat._data[13]);
	ret.setByIndex(2, _data[0] * mat._data[2] + _data[1] * mat._data[6] + _data[2] * mat._data[10] + _data[3] * mat._data[14]);
	ret.setByIndex(3, _data[0] * mat._data[3] + _data[1] * mat._data[7] + _data[2] * mat._data[11] + _data[3] * mat._data[15]);

	ret.setByIndex(4, _data[4] * mat._data[0] + _data[5] * mat._data[4] + _data[6] * mat._data[8] + _data[7] * mat._data[12]);
	ret.setByIndex(5, _data[4] * mat._data[1] + _data[5] * mat._data[5] + _data[6] * mat._data[9] + _data[7] * mat._data[13]);
	ret.setByIndex(6, _data[4] * mat._data[2] + _data[5] * mat._data[6] + _data[6] * mat._data[10] + _data[7] * mat._data[14]);
	ret.setByIndex(7, _data[4] * mat._data[3] + _data[5] * mat._data[7] + _data[6] * mat._data[11] + _data[7] * mat._data[15]);

	ret.setByIndex(8, _data[8] * mat._data[0] + _data[9] * mat._data[4] + _data[10] * mat._data[8] + _data[11] * mat._data[12]);
	ret.setByIndex(9, _data[8] * mat._data[1] + _data[9] * mat._data[5] + _data[10] * mat._data[9] + _data[11] * mat._data[13]);
	ret.setByIndex(10, _data[8] * mat._data[2] + _data[9] * mat._data[6] + _data[10] * mat._data[10] + _data[11] * mat._data[14]);
	ret.setByIndex(11, _data[8] * mat._data[3] + _data[9] * mat._data[7] + _data[10] * mat._data[11] + _data[11] * mat._data[15]);

	ret.setByIndex(12, _data[12] * mat._data[0] + _data[13] * mat._data[4] + _data[14] * mat._data[8] + _data[15] * mat._data[12]);
	ret.setByIndex(13, _data[12] * mat._data[1] + _data[13] * mat._data[5] + _data[14] * mat._data[9] + _data[15] * mat._data[13]);
	ret.setByIndex(14, _data[12] * mat._data[2] + _data[13] * mat._data[6] + _data[14] * mat._data[10] + _data[15] * mat._data[14]);
	ret.setByIndex(15, _data[12] * mat._data[3] + _data[13] * mat._data[7] + _data[14] * mat._data[11] + _data[15] * mat._data[15]);

	return ret;
}

Matrix4x4 Matrix4x4::operator+=(const Matrix4x4 & mat) {
	for (int i = 0; i < 16; i++)
		_data[i] += mat._data[i];
	return *this;
}

Matrix4x4 Matrix4x4::operator-=(const Matrix4x4 & mat) {
	for (int i = 0; i < 16; i++)
		_data[i] -= mat._data[i];
	return *this;
}

Matrix4x4 Matrix4x4::operator*=(float num) {
	for (int i = 0; i < 16; i++)
		_data[i] *= num;
	return *this;
}

bool Matrix4x4::operator==(const Matrix4x4 & mat) {
	for (int i = 0; i < 16; i++)
		if (_data[i] != mat._data[i])
			return false;
	return true;
}

bool Matrix4x4::operator!=(const Matrix4x4 & mat) {
	for (int i = 0; i < 16; i++)
		if (_data[i] != mat._data[i])
			return true;
	return false;
}

Matrix4x4 Matrix4x4::operator=(const Matrix4x4 & mat) {
	if (this != &mat) //Check for self-assignment
		for (int i = 0; i < 16; i++)
			_data[i] = mat._data[i];

	return *this;
}

std::ostream& operator<<(std::ostream &output, const Matrix4x4 &mat) {
	for (int i = 0; i < 16; i += 4)
		output << "(" << mat.getByIndex(i) << "  " << mat.getByIndex(i + 1) << "  " << mat.getByIndex(i + 2) << "  " << mat.getByIndex(i + 3) << ")" << std::endl;
	return output;
}

const void Matrix4x4::clean()
{
	for (int i = 0; i < 16; i++) {
		if (fabs(getByIndex(i)) < CLEANPREC) setByIndex(i, 0.0f);
	}
}