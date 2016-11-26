#ifndef Matrix2x2_H
#define Matrix2x2_H

#include <iostream>
#include "Vector2.h"

class Matrix2x2 {
private:
	float _data[4];
public:
	Matrix2x2() { for (int i = 0; i < 4; i++)_data[i] = 0; }
	Matrix2x2(float k) { for (int i = 0; i < 4; i++)_data[i] = k; }
	Matrix2x2(float x11, float x12, float x21, float x22) {
		_data[0] = x11;
		_data[1] = x12;
		_data[2] = x21;
		_data[3] = x22;
	}
	Matrix2x2(const Matrix2x2& mat) { for (int i = 0; i < 4; i++)_data[i] = mat._data[i]; }
	~Matrix2x2() {}
	float getByIndex(int i) const { return _data[i]; }
	float getByPos(int i, int j) const { return _data[(i - 1) * 2 + (j - 1)]; }
	void setByIndex(int i, float num) { _data[i] = num; }
	void setByPos(int i, int j, float num) { _data[(i - 1) * 2 + (j - 1)] = num; }

	float determinant();
	Matrix2x2 transpose();
	Matrix2x2 inverse();

	Matrix2x2 operator+(const Matrix2x2& mat);
	Matrix2x2 operator-(const Matrix2x2& mat);
	Matrix2x2 operator*(float num);
	Vector2 operator*(const Vector2& vec);
	Matrix2x2 operator*(const Matrix2x2& mat);
	Matrix2x2 operator+=(const Matrix2x2& mat);
	Matrix2x2 operator-=(const Matrix2x2& mat);
	Matrix2x2 operator*=(float num);
	bool operator==(const Matrix2x2& mat);
	bool operator!=(const Matrix2x2& mat);
	Matrix2x2 operator=(const Matrix2x2& mat);

	friend std::ostream& operator<<(std::ostream& output, const Vector2& mat);

};
Matrix2x2 operator*(float num, const Matrix2x2& mat);

#endif