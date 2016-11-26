#ifndef Vector4_H
#define Vector4_H

#include <iostream>

#define CLEANPREC 1.0e-5

class Vector4 {
private:
	float _x;
	float _y;
	float _z;
	float _w;
public:
	Vector4() : _x(0), _y(0), _z(0), _w(0) {}
	Vector4(float k) : _x(k), _y(k), _z(k), _w(k) {}
	Vector4(float x, float y, float z, float w) : _x(x), _y(y), _z(z), _w(w) {}
	Vector4(const Vector4& vec) : _x(vec._x), _y(vec._y), _z(vec._z), _w(vec._w) {}
	~Vector4() {}
	float getX() const { return _x; }
	float getY() const { return _y; }
	float getZ() const { return _z; }
	float getW() const { return _w; }
	float getNorm();//length
	float getQuad();
	void set(float x, float y, float z, float w);

	Vector4 operator+(const Vector4& vec);
	Vector4 operator-(const Vector4& vec);
	Vector4 operator*(float num);
	Vector4 operator+=(const Vector4& vec);
	Vector4 operator-=(const Vector4& vec);
	Vector4 operator*=(float num);
	bool operator==(const Vector4& vec);
	bool operator!=(const Vector4& vec);
	Vector4 operator=(const Vector4& vec);
	Vector4 normalize();
	Vector4 clean();

	float dotProduct(const Vector4& vec);
	Vector4 crossProduct(const Vector4& vec);

	friend std::ostream& operator<<(std::ostream& output, const Vector4& vec);
	friend std::istream& operator >> (std::istream& input, Vector4& vec);
};
Vector4 operator*(float num, const Vector4& vec);


#endif