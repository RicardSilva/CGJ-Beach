#pragma once

#include "Math.h"
#include <iostream>
#include <iomanip>

#define TOLERANCE 1.0e-6



namespace engine {

	struct vec2;
	struct vec3;
	struct vec4;

	struct vec2 {
		float x;
		float y;

		vec2() : x(0.0f), y(0.0f) {}
		vec2(float X, float Y) : x(X), y(Y) {}
		vec2(const vec2 &v) : x(v.x), y(v.y) {}

		void Set(float X, float Y) {
			x = X;
			y = Y;
		}

		friend std::ostream&  vec2::operator<<(std::ostream &os, vec2 &vec) {
			vec.Clean();
			os << '(' << vec.x << ',' << vec.y << ')';
			return os;
		}
		friend std::istream&  vec2::operator>>(std::istream &is, vec2 &vec) {
			is >> vec.x >> vec.y;
			return is;
		}

		friend bool operator==(const vec2 &v1, const vec2 &v2) {
			return (fabs(v1.x - v2.x) < TOLERANCE
				&&  fabs(v1.y - v2.y) < TOLERANCE);
		}
		friend bool operator!=(const vec2 &v1, const vec2 &v2) {
			return !(v1 == v2);
		}

		friend vec2 operator+(const vec2 &v1, const vec2 &v2) {
			return vec2(v1.x + v2.x, v1.y + v2.y);
		}
		friend vec2 operator-(const vec2 &v1, const vec2 &v2) {
			return vec2(v1.x - v2.x, v1.y - v2.y);
		}
		friend vec2 operator*(const vec2 &v, float scalar) {
			return vec2(v.x*scalar, v.y*scalar);
		}
		friend vec2 operator*(float scalar, const vec2 &v) {
			return vec2(v.x*scalar, v.y*scalar);
		}
		friend vec2 operator*(const vec2 &v, const vec2 &v2) {
			return vec2(v.x*v2.x, v.y*v2.y);
		}
		friend vec2 operator/(const vec2 &v, float scalar) {
			scalar = 1.0f / scalar;
			return vec2(v.x*scalar, v.y*scalar);
		}

		vec2 &operator = (const vec2 &v);
		vec2 &operator += (const vec2 &v);
		vec2 &operator -= (const vec2 &v);
		vec2 &operator *= (float scalar);
		vec2 &operator /= (float scalar);

		friend float DotProduct(const vec2 &v1, const vec2 &v2) {
			return v1.x * v2.x + v1.y * v2.y;
		}

		void Clean();
		void MakeZero();
		bool IsZero();

		float sqrMagnitude();
		float Magnitude();
		void Normalize();
		vec2 Normalized();

		const float* Export() const;
	};


	struct vec3 {
		float x;
		float y;
		float z;

		vec3() : x(0.0f), y(0.0f), z(0.0f) {}
		vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
		vec3(const vec3 &v) : x(v.x), y(v.y), z(v.z) {}

		void Set(float X, float Y, float Z) {
			x = X;
			y = Y;
			z = Z;
		}

		friend std::ostream&  vec3::operator<<(std::ostream &os, vec3 &vec) {
			vec.Clean();
			os << '(' << vec.x << ',' << ' ' << vec.y << ',' << ' '<< vec.z << ')';
			return os;
		}
		friend std::istream&  vec3::operator>>(std::istream &is, vec3 &vec) {
			is >> vec.x >> vec.y >> vec.z;
			return is;
		}

		friend bool operator==(const vec3 &v1, const vec3 &v2) {
			return (fabs(v1.x - v2.x) < TOLERANCE
				&&  fabs(v1.y - v2.y) < TOLERANCE
				&&  fabs(v1.z - v2.z) < TOLERANCE);
		}
		friend bool operator!=(const vec3 &v1, const vec3 &v2) {
			return !(v1 == v2);
		}

		friend vec3 operator+(const vec3 &v1, const vec3 &v2) {
			return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
		}
		friend vec3 operator-(const vec3 &v1, const vec3 &v2) {
			return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
		}
		friend vec3 operator*(const vec3 &v, float scalar) {
			return vec3(v.x*scalar, v.y*scalar, v.z*scalar);
		}
		friend vec3 operator*(float scalar, const vec3 &v) {
			return vec3(v.x*scalar, v.y*scalar, v.z*scalar);
		}
		friend vec3 operator/(const vec3 &v, float scalar) {
			scalar = 1.0f / scalar;
			return vec3(v.x*scalar, v.y*scalar, v.z*scalar);
		}
	
		vec3 &operator = (const vec3 &v);
		vec3 &operator += (const vec3 &v);
		vec3 &operator -= (const vec3 &v);
		vec3 &operator *= (float scalar);
		vec3 &operator /= (float scalar);


		friend float DotProduct(const vec3 &v1, const vec3 &v2) {
			return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		}
		friend vec3 CrossProduct(const vec3 &v1, const vec3 &v2) {
			return vec3(v1.y*v2.z - v1.z*v2.y,
						v1.z*v2.x - v1.x*v2.z,
						v1.x*v2.y - v1.y*v2.x);
		}
		friend vec3 Lerp(const vec3 &v1, const vec3 &v2, float k) {
			return (1 - k) * v1 + k * v2;
		}

		void Clean();
		void MakeZero();
		bool IsZero();

		float sqrMagnitude();
		float Magnitude();
		void Normalize();
		vec3 Normalized();

		const float* Export() const;
	};

	struct vec4 {
		float x;
		float y;
		float z;
		float w;

		vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		vec4(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W) {}
		vec4(const vec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
		vec4(const vec3 &v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

		void Set(float X, float Y, float Z, float W) {
			x = X;
			y = Y;
			z = Z;
			w = W;
		}

		friend std::ostream&  vec4::operator<<(std::ostream &os, vec4 &vec) {
			vec.Clean();
			os << '(' << vec.x << ',' << ' ' << vec.y << ',' << ' ' << vec.z << ',' << ' ' << vec.w << ')';
			return os;
		}
		friend std::istream&  vec4::operator>>(std::istream &is, vec4 &vec) {
			is >> vec.x >> vec.y >> vec.z >> vec.w;
			return is;
		}

		friend bool operator==(const vec4 &v1, const vec4 &v2) {
			return (fabs(v1.x - v2.x) < TOLERANCE
				&&  fabs(v1.y - v2.y) < TOLERANCE
				&&  fabs(v1.z - v2.z) < TOLERANCE
				&&  fabs(v1.w - v2.w) < TOLERANCE);
		}
		friend bool operator!=(const vec4 &v1, const vec4 &v2) {
			return !(v1 == v2);
		}

		friend vec4 operator+(const vec4 &v1, const vec4 &v2) {
			return vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
		}
		friend vec4 operator-(const vec4 &v1, const vec4 &v2) {
			return vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
		}
		friend vec4 operator*(const vec4 &v, float scalar) {
			return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
		}
		friend vec4 operator*(float scalar, const vec4 &v) {
			return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
		}
		friend vec4 operator/(const vec4 &v, float scalar) {
			scalar = 1.0f / scalar;
			return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
		}

		vec4 &operator = (const vec4 &v);
		vec4 &operator += (const vec4 &v);
		vec4 &operator -= (const vec4 &v);
		vec4 &operator *= (float scalar);
		vec4 &operator /= (float scalar);

		void Clean();
		void MakeZero();
		bool IsZero();

		float sqrMagnitude();
		float Magnitude();
		void Normalize();
		vec4 Normalized();

		const float* Export() const;
	};

}