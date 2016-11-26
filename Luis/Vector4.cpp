#include "Vector4.h"

float Vector4::getNorm() {
	return getW() * sqrt(pow(getX(), 2) + pow(getY(), 2) + pow(getZ(), 2));
}

float Vector4::getQuad() {
	return (pow(getX(), 2) + pow(getY(), 2) + pow(getZ(), 2));
}

void Vector4::set(float x, float y, float z, float w) {
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

Vector4 Vector4::operator+(const Vector4& vec) {
	Vector4 vector = *this;
	vector += vec;
	return vector;
}

Vector4 Vector4::operator-(const Vector4& vec) {
	Vector4 vector = *this;
	vector -= vec;
	return vector;
}

Vector4 Vector4::operator*(float num) {
	Vector4 vector = *this;
	vector.set(_x * num, _y * num, _z * num, _w * num);
	return vector;
}

Vector4 operator*(float num, const Vector4 & vec)
{
	Vector4 vector = vec;
	vector.set(vector.getX() * num, vector.getY() * num, vector.getZ() * num, vector.getW() * num);
	return vector;
}

Vector4 Vector4::operator+=(const Vector4& vec) {
	_x += vec._x;
	_y += vec._y;
	_z += vec._z;
	_w += vec._w;
	return *this;
}

Vector4 Vector4::operator-=(const Vector4& vec) {
	_x -= vec._x;
	_y -= vec._y;
	_z -= vec._z;
	_w -= vec._w;
	return *this;
}

Vector4 Vector4::operator*=(float num) {
	_x = _x * num;
	_y = _y * num;
	_z = _z * num;
	_w = _w * num;
	return *this;
}

bool Vector4::operator==(const Vector4& vec) {
	if (_x == vec._x && _y == vec._y && _z == vec._z && _w == vec._w) return true;
	return false;
}

bool Vector4::operator!=(const Vector4& vec) {
	if (_x != vec._x || _y != vec._y || _z != vec._z || _w != vec._w) return true;
	return false;
}

Vector4 Vector4::operator=(const Vector4& vec) {
	if (this != &vec) //Check for self-assignment
		set(vec._x, vec._y, vec._z, vec._w);

	return *this;
}

Vector4 Vector4::normalize() {
	float norm = this->getNorm();
	return Vector4(_x / norm, _y / norm, _z / norm, 1.0f);
}

Vector4 Vector4::clean() {
	if (_x < CLEANPREC && _x > -CLEANPREC) _x = 0.0f;
	if (_y < CLEANPREC && _y > -CLEANPREC) _y = 0.0f;
	if (_z < CLEANPREC && _z > -CLEANPREC) _z = 0.0f;
	if (_w < CLEANPREC && _w > -CLEANPREC) _w = 0.0f;
	return *this;
};

float Vector4::dotProduct(const Vector4& vec) {
	return ((_x * vec._x) + (_y * vec._y) + (_z * vec._z) + (_w * vec._w));
}

std::ostream& operator<<(std::ostream &output, const Vector4 &vec) {
	output << "(" << vec._x << ", " << vec._y << ", " << vec._z << ", " << vec._w << ")" << std::endl;
	return output;
}

std::istream& operator >> (std::istream &input, Vector4 &vec) {
	input >> vec._x >> vec._y >> vec._z >> vec._w;
	return input;
}
