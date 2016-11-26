#include "Matrix2x2.h"

float Matrix2x2::determinant(){
	return _data[0] * _data[3] - _data[2] * _data[1];
}

Matrix2x2 Matrix2x2::transpose() {
	return Matrix2x2(_data[0], _data[2], _data[1], _data[3]);
}

Matrix2x2 Matrix2x2::inverse() {
	Matrix2x2 mat = Matrix2x2(_data[3], -_data[1], -_data[2], _data[0]);
	mat *= this->determinant();
	return mat;
}

Matrix2x2 Matrix2x2::operator+(const Matrix2x2 & mat) {
	Matrix2x2 matrix = *this;
	matrix += mat;
	return matrix;
}

Matrix2x2 Matrix2x2::operator-(const Matrix2x2 & mat) {
	Matrix2x2 matrix = *this;
	matrix -= mat;
	return matrix;
}

Matrix2x2 Matrix2x2::operator*(float num) {
	Matrix2x2 matrix = *this;
	matrix *= num;
	return matrix;
}

Vector2 Matrix2x2::operator*(const Vector2& vec) {
	float x = _data[0] * vec.getX() + _data[1] * vec.getY();
	float y = _data[2] * vec.getX() + _data[3] * vec.getY();
	return Vector2(x, y);
}

Matrix2x2 operator*(float num, const Matrix2x2& mat) {
	Matrix2x2 matrix = mat;
	matrix *= num;
	return matrix;
}


Matrix2x2 Matrix2x2::operator*(const Matrix2x2& mat) {
	Matrix2x2 ret = Matrix2x2();
	ret.setByIndex(0, _data[0] * mat._data[0] + _data[1] * mat._data[2]);
	ret.setByIndex(1, _data[0] * mat._data[1] + _data[1] * mat._data[3]);

	ret.setByIndex(2, _data[2] * mat._data[0] + _data[3] * mat._data[2]);
	ret.setByIndex(3, _data[2] * mat._data[1] + _data[3] * mat._data[3]);

	return ret;
}

Matrix2x2 Matrix2x2::operator+=(const Matrix2x2 & mat) {
	for (int i = 0; i < 4; i++)
		_data[i] += mat._data[i];
	return *this;
}

Matrix2x2 Matrix2x2::operator-=(const Matrix2x2 & mat) {
	for (int i = 0; i < 4; i++)
		_data[i] -= mat._data[i];
	return *this;
}

Matrix2x2 Matrix2x2::operator*=(float num) {
	for (int i = 0; i < 4; i++)
		_data[i] *= num;
	return *this;
}

bool Matrix2x2::operator==(const Matrix2x2 & mat) {
	for (int i = 0; i < 4; i++)
		if (_data[i] != mat._data[i])
			return false;
	return true;
}

bool Matrix2x2::operator!=(const Matrix2x2 & mat) {
	for (int i = 0; i < 4; i++)
		if (_data[i] != mat._data[i])
			return true;
	return false;
}

Matrix2x2 Matrix2x2::operator=(const Matrix2x2 & mat) {
	if (this != &mat) //Check for self-assignment
		for (int i = 0; i < 4; i++)
			_data[i] = mat._data[i];

	return *this;
}

std::ostream& operator<<(std::ostream &output, const Matrix2x2 &mat) {
	for (int i = 0; i < 4; i += 2)
		output << "(" << mat.getByIndex(i) << "  " << mat.getByIndex(i + 1) << ")" << std::endl;
	return output;
}