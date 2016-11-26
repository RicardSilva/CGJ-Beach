#include "Vector2.h"

#define CLEANPREC 0.0001

float Vector2::getNorm() {
	return sqrt(pow(getX(), 2) + pow(getY(), 2));
}

float Vector2::getQuad() {
	return (pow(getX(), 2) + pow(getY(), 2));
}

void Vector2::set(float x, float y) {
	_x = x;
	_y = y;
}

Vector2 Vector2::operator+(const Vector2& vec) {
	Vector2 vector = *this;
	vector += vec;
	return vector;
}

Vector2 Vector2::operator-(const Vector2& vec) {
	Vector2 vector = *this;
	vector -= vec;
	return vector;
}

Vector2 Vector2::operator*(float num) {
	Vector2 vector = *this;
	vector.set(_x * num, _y * num);
	return vector;
}

Vector2 operator*(float num, const Vector2& vec)
{
	Vector2 vector = vec;
	vector.set(vector.getX() * num, vector.getY() * num);
	return vector;
}

Vector2 Vector2::operator+=(const Vector2& vec) {
	_x += vec._x;
	_y += vec._y;
	return *this;
}

Vector2 Vector2::operator-=(const Vector2& vec) {
	_x -= vec._x;
	_y -= vec._y;
	return *this;
}

Vector2 Vector2::operator*=(float num) {
	_x = _x * num;
	_y = _y * num;
	return *this;
}

bool Vector2::operator==(const Vector2& vec) {
	if(_x == vec._x && _y == vec._y) return true;
	return false;
}

bool Vector2::operator!=(const Vector2& vec) {
	if(_x != vec._x || _y != vec._y) return true;
	return false;
}

Vector2 Vector2::operator=(const Vector2& vec) {
	if (this != &vec) //Check for self-assignment
		set(vec._x, vec._y);

	return *this;
}

Vector2 Vector2::normalize() {
	float norm = this->getNorm();
	_x = _x / norm;
	_y = _y / norm;
	return *this; 
}

Vector2 Vector2::clean() {
	if (_x < CLEANPREC && _x > -CLEANPREC) _x = 0.0f;
	if (_y < CLEANPREC && _y > -CLEANPREC) _y = 0.0f;
	return *this;
};

std::ostream& operator<<(std::ostream &output, const Vector2 &vec) { 
	output << "(" << vec._x << ", " << vec._y << ")" << std::endl;
	return output;            
}

std::istream& operator>>(std::istream &input, Vector2 &vec) { 
	input >> vec._x >> vec._y;
	return input;            
}
