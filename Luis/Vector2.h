#ifndef Vector2_H
#define Vector2_H

#include <iostream>

class Vector2 {
	private:
		float _x;
		float _y;
	public:
		Vector2(): _x(0), _y(0) {}
		Vector2(float k): _x(k), _y(k) {}
		Vector2(float x, float y): _x(x), _y(y) {}
		Vector2(const Vector2& vec): _x(vec._x), _y(vec._y) {}
		~Vector2(){}
		float getX() const { return _x; }
		float getY() const { return _y; }
		float getNorm();//length
		float getQuad();
		void set(float x, float y);
		
		Vector2 operator+(const Vector2& vec);
		Vector2 operator-(const Vector2& vec);
		Vector2 operator*(float num);
		Vector2 operator+=(const Vector2& vec);
		Vector2 operator-=(const Vector2& vec);
		Vector2 operator*=(float num);
		bool operator==(const Vector2& vec);
		bool operator!=(const Vector2& vec);
		Vector2 operator=(const Vector2& vec);
		Vector2 normalize();
		Vector2 Vector2::clean();

		friend std::ostream& operator<<(std::ostream& output, const Vector2& vec);
		friend std::istream& operator>>(std::istream& input, Vector2& vec);
};
Vector2 operator*(float num, const Vector2& vec);

#endif