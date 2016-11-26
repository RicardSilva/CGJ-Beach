#ifndef Vector3_H
#define Vector3_H

#include <iostream>

class Vector3 {
	private:
		float _x;
		float _y;
		float _z;
	public:
		Vector3(): _x(0), _y(0),_z(0) {}
		Vector3(float k): _x(k), _y(k), _z(k) {}
		Vector3(float x, float y, float z): _x(x), _y(y), _z(z) {}
		Vector3(const Vector3& vec): _x(vec._x), _y(vec._y), _z(vec._z) {}
		~Vector3(){}
		float getX() const { return _x; }
		float getY() const { return _y; }
		float getZ() const { return _z; }
		float getNorm();//length
		float getQuad();
		void set(float x, float y, float z);
		
		Vector3 operator+(const Vector3& vec);
		Vector3 operator-(const Vector3& vec);
		Vector3 operator*(float num);
		Vector3 operator+=(const Vector3& vec);
		Vector3 operator-=(const Vector3& vec);
		Vector3 operator*=(float num);
		bool operator==(const Vector3& vec);
		bool operator!=(const Vector3& vec);
		Vector3 operator=(const Vector3& vec);
		Vector3 normalize();
		Vector3 clean();
		
		float dotProduct(const Vector3& vec);
		Vector3 crossProduct(const Vector3& vec);
		
		friend std::ostream& operator<<(std::ostream& output, const Vector3& vec);
		friend std::istream& operator>>(std::istream& input, Vector3& vec);
};
Vector3 operator*(float num, const Vector3& vec);


#endif