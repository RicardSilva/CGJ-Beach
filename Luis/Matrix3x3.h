#ifndef Matrix3x3_H
#define Matrix3x3_H

#include <iostream>
#include "Vector3.h"

class Matrix3x3 {
private:
	float _data[9];
public:
	Matrix3x3(){for (int i = 0; i < 9; i++)_data[i] = 0;}
	Matrix3x3(float k){ for (int i = 0; i < 9; i++)_data[i] = k; }
	Matrix3x3(float x11, float x12, float x13, float x21, float x22, float x23, float x31, float x32, float x33){
		_data[0] = x11;
		_data[1] = x12;
		_data[2] = x13;
		_data[3] = x21;
		_data[4] = x22;
		_data[5] = x23;
		_data[6] = x31;
		_data[7] = x32;
		_data[8] = x33;
	}
	Matrix3x3(const Matrix3x3& mat) { for (int i = 0; i < 9; i++)_data[i] = mat._data[i]; }
	~Matrix3x3(){}
	float getByIndex(int i) const { return _data[i]; }
	float getByPos(int i, int j) const { return _data[(i - 1) * 3 + (j - 1)]; }
	void setByIndex(int i, float num) { _data[i] = num; }
	void setByPos(int i, int j, float num){ _data[(i - 1) * 3 + (j - 1)] = num; }

	float determinant();
	Matrix3x3 transpose();
	Matrix3x3 inverse();

	Matrix3x3 operator+(const Matrix3x3& mat);
	Matrix3x3 operator-(const Matrix3x3& mat);
	Matrix3x3 operator*(float num);
	Vector3 operator*(const Vector3& vec);
	Matrix3x3 operator*(const Matrix3x3& mat);
	Matrix3x3 operator+=(const Matrix3x3& mat);
	Matrix3x3 operator-=(const Matrix3x3& mat);
	Matrix3x3 operator*=(float num);
	bool operator==(const Matrix3x3& mat);
	bool operator!=(const Matrix3x3& mat);
	Matrix3x3 operator=(const Matrix3x3& mat);

	Matrix3x3 Matrix3x3::clean();

	friend std::ostream& operator<<(std::ostream& output, const Matrix3x3& mat);

};
Matrix3x3 operator*(float num, const Matrix3x3& mat);

#endif