#include "Matrix3x3.h"

#define CLEANPREC 0.0001

float Matrix3x3::determinant() {
	float det = _data[0] * (_data[4] * _data[8] - _data[7] * _data[5]);
	det += -_data[1] * (_data[3] * _data[8] - _data[6] * _data[5]);
	det += _data[2] * (_data[3] * _data[7] - _data[6] * _data[4]);
	return det;
}

Matrix3x3 Matrix3x3::transpose(){
	return Matrix3x3(_data[0], _data[3], _data[6], _data[1], _data[4], _data[7], _data[2], _data[5], _data[8]);
}

Matrix3x3 Matrix3x3::inverse() {
	Matrix3x3 mat = Matrix3x3();
	mat.setByIndex(0, _data[4] * _data[8] - _data[7] * _data[5]);
	mat.setByIndex(1, -(_data[3] * _data[8] - _data[6] * _data[5]));
	mat.setByIndex(2, _data[3] * _data[7] - _data[6] * _data[4]);

	mat.setByIndex(3, -(_data[1] * _data[8] - _data[7] * _data[2]));
	mat.setByIndex(4, _data[0] * _data[8] - _data[6] * _data[2]);
	mat.setByIndex(5, -(_data[0] * _data[7] - _data[6] * _data[1]));

	mat.setByIndex(6, _data[1] * _data[5] - _data[4] * _data[2]);
	mat.setByIndex(7, -(_data[0] * _data[5] - _data[3] * _data[2]));
	mat.setByIndex(8, _data[0] * _data[4] - _data[3] * _data[1]);

	mat = mat.transpose();
	mat *= 1 /this->determinant();
	return mat;
}

Matrix3x3 Matrix3x3::operator+(const Matrix3x3 & mat) {
	Matrix3x3 matrix = *this;
	matrix += mat;
	return matrix;
}

Matrix3x3 Matrix3x3::operator-(const Matrix3x3 & mat) {
	Matrix3x3 matrix = *this;
	matrix -= mat;
	return matrix;
}

Matrix3x3 Matrix3x3::operator*(float num) {
	Matrix3x3 matrix = *this;
	matrix *= num;
	return matrix;
}

Vector3 Matrix3x3::operator*(const Vector3& vec) {
	float x = _data[0] * vec.getX() + _data[1] * vec.getY() + _data[2] * vec.getZ();
	float y = _data[3] * vec.getX() + _data[4] * vec.getY() + _data[5] * vec.getZ();
	float z = _data[6] * vec.getX() + _data[7] * vec.getY() + _data[8] * vec.getZ();
	return Vector3(x, y, z);
}

Matrix3x3 operator*(float num, const Matrix3x3& mat) {
	Matrix3x3 matrix = mat;
	matrix *= num;
	return matrix;
}


Matrix3x3 Matrix3x3::operator*(const Matrix3x3& mat) {
	Matrix3x3 ret = Matrix3x3();
	ret.setByIndex(0, _data[0] * mat._data[0] + _data[1] * mat._data[3] + _data[2] * mat._data[6]);
	ret.setByIndex(1, _data[0] * mat._data[1] + _data[1] * mat._data[4] + _data[2] * mat._data[7]);
	ret.setByIndex(2, _data[0] * mat._data[2] + _data[1] * mat._data[5] + _data[2] * mat._data[8]);

	ret.setByIndex(3, _data[3] * mat._data[0] + _data[4] * mat._data[3] + _data[5] * mat._data[6]);
	ret.setByIndex(4, _data[3] * mat._data[1] + _data[4] * mat._data[4] + _data[5] * mat._data[7]);
	ret.setByIndex(5, _data[3] * mat._data[2] + _data[4] * mat._data[5] + _data[5] * mat._data[8]);

	ret.setByIndex(6, _data[6] * mat._data[0] + _data[7] * mat._data[3] + _data[8] * mat._data[6]);
	ret.setByIndex(7, _data[6] * mat._data[1] + _data[7] * mat._data[4] + _data[8] * mat._data[7]);
	ret.setByIndex(8, _data[6] * mat._data[2] + _data[7] * mat._data[5] + _data[8] * mat._data[8]);
	return ret;
}

Matrix3x3 Matrix3x3::operator+=(const Matrix3x3 & mat) {
	for (int i = 0; i < 9; i++)
		_data[i] += mat._data[i];
	return *this;
}

Matrix3x3 Matrix3x3::operator-=(const Matrix3x3 & mat) {
	for (int i = 0; i < 9; i++)
		_data[i] -= mat._data[i];
	return *this;
}

Matrix3x3 Matrix3x3::operator*=(float num) {
	for (int i = 0; i < 9; i++)
		_data[i] *= num;
	return *this;
}

bool Matrix3x3::operator==(const Matrix3x3 & mat) {
	for (int i = 0; i < 9; i++)
		if(_data[i] != mat._data[i])
			return false;
	return true;
}

bool Matrix3x3::operator!=(const Matrix3x3 & mat) {
	for (int i = 0; i < 9; i++)
		if (_data[i] != mat._data[i])
			return true;
	return false;
}

Matrix3x3 Matrix3x3::operator=(const Matrix3x3 & mat) {
	if (this != &mat) //Check for self-assignment
		for (int i = 0; i < 9; i++)
			_data[i] = mat._data[i];

	return *this;
}

Matrix3x3 Matrix3x3::clean() {
	for (int i = 0; i < 9; i++)
		if (_data[i] < CLEANPREC && _data[i] > -CLEANPREC) _data[i] = 0.0f;
	return *this;
};

std::ostream& operator<<(std::ostream &output, const Matrix3x3 &mat) {
	for (int i = 0; i < 9; i += 3)
		output << "(" << mat.getByIndex(i) << "  " << mat.getByIndex(i + 1) << "  " << mat.getByIndex(i + 2) << ")" << std::endl;
	return output;
}