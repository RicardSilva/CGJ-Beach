#ifndef Matrix4x4_H
#define Matrix4x4_H

#include <iostream>
#include "Vector4.h"

class Matrix4x4 {
private:
	float _data[16];
public:
	Matrix4x4() { for (int i = 0; i < 16; i++)_data[i] = 0; }
	Matrix4x4(float k) { for (int i = 0; i < 16; i++)_data[i] = k; }
	Matrix4x4(float x11, float x12, float x13, float x14, float x21, float x22, float x23, float x24, float x31, float x32, float x33, float x34, float x41, float x42, float x43, float x44) {
		_data[0] = x11;
		_data[1] = x12;
		_data[2] = x13;
		_data[3] = x14;
		_data[4] = x21;
		_data[5] = x22;
		_data[6] = x23;
		_data[7] = x24;
		_data[8] = x31;
		_data[9] = x32;
		_data[10] = x33;
		_data[11] = x34;
		_data[12] = x41;
		_data[13] = x42;
		_data[14] = x43;
		_data[15] = x44;

	}
	Matrix4x4(const Matrix4x4& mat) { for (int i = 0; i < 16; i++)_data[i] = mat._data[i]; }
	~Matrix4x4() {}
	float getByIndex(int i) const { return _data[i]; }
	float getByPos(int i, int j) const { return _data[(i - 1) * 4 + (j - 1)]; }
	void setByIndex(int i, float num) { _data[i] = num; }
	void setByPos(int i, int j, float num) { _data[(i - 1) * 4 + (j - 1)] = num; }

	float* getData() { return _data; }

	Matrix4x4 transpose();

	Matrix4x4 operator+(const Matrix4x4& mat);
	Matrix4x4 operator-(const Matrix4x4& mat);
	Matrix4x4 operator*(float num);
	Vector4 Matrix4x4::operator*(const Vector4& vec);
	Matrix4x4 operator*(const Matrix4x4& mat);
	Matrix4x4 operator+=(const Matrix4x4& mat);
	Matrix4x4 operator-=(const Matrix4x4& mat);
	Matrix4x4 operator*=(float num);
	bool operator==(const Matrix4x4& mat);
	bool operator!=(const Matrix4x4& mat);
	Matrix4x4 operator=(const Matrix4x4& mat);

	friend std::ostream& operator<<(std::ostream& output, const Matrix4x4& mat);

	const void clean();

};
Matrix4x4 operator*(float num, const Matrix4x4& mat);

#endif