#include "Vector3.h"

#define CLEANPREC 0.0001

float Vector3::getNorm() {
	return sqrt(pow(getX(), 2) + pow(getY(), 2) + pow(getZ(), 2));
}

float Vector3::getQuad() {
	return (pow(getX(), 2) + pow(getY(), 2)+ pow(getZ(), 2));
}

void Vector3::set(float x, float y, float z) {
	_x = x;
	_y = y;
	_z = z;
}

Vector3 Vector3::operator+(const Vector3& vec) {
	Vector3 vector = *this;
	vector += vec;
	return vector;
}

Vector3 Vector3::operator-(const Vector3& vec) {
	Vector3 vector = *this;
	vector -= vec;
	return vector;
}

Vector3 Vector3::operator*(float num) {
	Vector3 vector = *this;
	vector.set(_x * num, _y * num, _z * num);
	return vector;
}

Vector3 operator*(float num, const Vector3 & vec)
{
	Vector3 vector = vec;
	vector.set(vector.getX() * num, vector.getY() * num, vector.getZ() * num);
	return vector;
}

Vector3 Vector3::operator+=(const Vector3& vec) {
	_x += vec._x;
	_y += vec._y;
	_z += vec._z;
	return *this;
}

Vector3 Vector3::operator-=(const Vector3& vec) {
	_x -= vec._x;
	_y -= vec._y;
	_z -= vec._z;
	return *this;
}

Vector3 Vector3::operator*=(float num) {
	_x = _x * num;
	_y = _y * num;
	_z = _z * num;
	return *this;
}

bool Vector3::operator==(const Vector3& vec) {
	if(_x == vec._x && _y == vec._y && _z == vec._z) return true;
	return false;
	
	/*return fabs(v0.x - v1.x) THRESHOLD &&
			fabs(v0.y - v1.y) THRESHOLD &&
			fabs(v0.z - v1.z) THRESHOLD)*/
}

bool Vector3::operator!=(const Vector3& vec) {
	if(_x != vec._x || _y != vec._y || _z != vec._z) return true;
	return false;
}

Vector3 Vector3::operator=(const Vector3& vec) {
	if (this != &vec) //Check for self-assignment
		set(vec._x, vec._y, vec._z);

	return *this;
}

Vector3 Vector3::normalize() {
	float norm = this->getNorm();
	return Vector3(_x / norm, _y / norm, _z / norm);
}

Vector3 Vector3::clean() {
	if (_x < CLEANPREC && _x > -CLEANPREC) _x = 0.0f;
	if (_y < CLEANPREC && _y > -CLEANPREC) _y = 0.0f;
	if (_z < CLEANPREC && _z > -CLEANPREC) _z = 0.0f;
	return *this;
};

float Vector3::dotProduct(const Vector3& vec){
	return ((_x * vec._x) + (_y * vec._y) + (_z * vec._z));
}

Vector3 Vector3::crossProduct(const Vector3& vec){
	return Vector3(((_y * vec._z) - (_z * vec._y)), ((_z * vec._x) - (_x * vec._z)), ((_x * vec._y) - (_y * vec._x)));
}

std::ostream& operator<<(std::ostream &output, const Vector3 &vec) { 
	output << "(" << vec._x << ", " << vec._y << ", " << vec._z << ")" << std::endl;
	return output;
}

std::istream& operator>>(std::istream &input, Vector3 &vec) { 
	input >> vec._x >> vec._y >> vec._z;
	return input;            
}
