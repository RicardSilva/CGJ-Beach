#pragma once

#include "Math.h"
#include "vec.h"
#include <iostream>
#include <iomanip>


#define TOLERANCE 1.0e-6
#define PI 3.14159265f

namespace engine {

	struct mat2;
	struct mat3;
	struct mat4;
	struct matFactory;


	struct mat2 {
		float data[4];

		float& mat2::operator[] (const int index) {
			return data[index];
		}
		const float& mat2::operator[] (const int index) const {
			return data[index];
		}

		mat2() {
			data[0] = 0.0f;
			data[1] = 0.0f;
			data[2] = 0.0f;
			data[3] = 0.0f;
		}
		mat2(float a, float b,
			 float c, float d) {
			data[0] = a;
			data[1] = b;
			data[2] = c;
			data[3] = d;
		}
		mat2(const mat2 &m) {
			data[0] = m[0];
			data[1] = m[1];
			data[2] = m[2];
			data[3] = m[3];
		}

		friend std::ostream&  mat2::operator<<(std::ostream &os, mat2 &m) {
			m.Clean();
			os << '[' << m.data[0] << ' ' << m.data[1] << std::endl << ' ' << m.data[2] << ' ' << m.data[3] << ']';
			return os;
		}
		friend std::istream&  mat2::operator>>(std::istream &is, mat2 &m) {
			is >> m.data[0] >> m.data[1] >> m.data[2] >> m.data[3];
			return is;
		}

		friend bool operator==(const mat2 &m1, const mat2 &m2) {
			return (fabs(m1[0] - m2[0]) < TOLERANCE
				&&  fabs(m1[1] - m2[1]) < TOLERANCE
				&&  fabs(m1[2] - m2[2]) < TOLERANCE
				&&  fabs(m1[3] - m2[3]) < TOLERANCE);
		}
		friend bool operator!=(const mat2 &m1, const mat2 &m2) {
			return !(m1 == m2);
		}

		friend mat2 operator+(const mat2 &m1, const mat2 &m2) {
			return mat2(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2], m1[3] + m2[3]);
		}
		friend mat2 operator-(const mat2 &m1, const mat2 &m2) {
			return mat2(m1[0] - m2[0], m1[1] - m2[1], 
				        m1[2] - m2[2], m1[3] - m2[3]);
		}
		friend mat2 operator*(const mat2 &m, float scalar) {
			return mat2(m[0] * scalar, m[1] * scalar,
				        m[2] * scalar, m[3] * scalar);
		}
		friend mat2 operator*(float scalar, const mat2 &m) {
			return mat2(m[0] * scalar, m[1] * scalar,
				        m[2] * scalar, m[3] * scalar);
		}
		friend mat2 operator*(const mat2 &m1, const mat2 &m2) {
			return mat2(m1[0] * m2[0] + m1[1] * m2[2], m1[0] * m2[1] + m1[1] * m2[3],
				        m1[2] * m2[0] + m1[3] * m2[2], m1[2] * m2[1] + m1[3] * m2[3]);
		}
		friend vec2 operator*(const mat2 &m, const vec2 &v) {
			return vec2(m[0] * v.x + m[1] * v.y, m[2] * v.x + m[3] * v.y);
		}
		

		mat2 &operator = (const mat2 &v);
		mat2 &operator += (const mat2 &v);
		mat2 &operator -= (const mat2 &v);
		mat2 &operator *= (float scalar);
		

		void Clean();
		void MakeZero();

		float Determinant();
		void Inverse();
		mat2 Inversed();
		void Transpose();
		mat2 Transposed();

		const float* Export();
	};

	struct mat3 {
		float data[9];

		float& mat3::operator[] (const int index) {
			return data[index];
		}
		const float& mat3::operator[] (const int index) const {
			return data[index];
		}

		mat3() {
			data[0] = 0.0f;	data[1] = 0.0f;	data[2] = 0.0f;
			data[3] = 0.0f;	data[4] = 0.0f;	data[5] = 0.0f;
			data[6] = 0.0f;	data[7] = 0.0f;	data[8] = 0.0f;
		}
			
		mat3(float a, float b, float c, 
			 float d, float e, float f, 
			 float g, float h, float i) {

			data[0] = a;	data[1] = b;	data[2] = c;
			data[3] = d;	data[4] = e;	data[5] = f;
			data[6] = g;	data[7] = h;	data[8] = i;
		}
		mat3(const mat3 &m) {
			data[0] = m[0]; data[1] = m[1]; data[2] = m[2];
			data[3] = m[3]; data[4] = m[4]; data[5] = m[5];
			data[6] = m[6]; data[7] = m[7]; data[8] = m[8];
		}

		friend std::ostream&  mat3::operator<<(std::ostream &os, mat3 &m) {
			m.Clean();
			os << '[' << m[0] << ' ' << m[1] << ' ' << m[2] << std::endl;
			os << ' ' << m[3] << ' ' << m[4] << ' ' << m[5] << std::endl;
			os << ' ' << m[6] << ' ' << m[7] << ' ' << m[8] << ']';
			return os;
		}
		friend std::istream&  mat3::operator>>(std::istream &is, mat3 &m) {
			is >> m[0] >> m[1] >> m[2];
			is >> m[3] >> m[4] >> m[5];
			is >> m[6] >> m[7] >> m[8];
			return is;
		}

		friend bool operator==(const mat3 &m1, const mat3 &m2) {
			return (fabs(m1[0] - m2[0]) < TOLERANCE
				&&  fabs(m1[1] - m2[1]) < TOLERANCE
				&&  fabs(m1[2] - m2[2]) < TOLERANCE
				&&  fabs(m1[3] - m2[3]) < TOLERANCE
				&&  fabs(m1[4] - m2[4]) < TOLERANCE
				&&  fabs(m1[5] - m2[5]) < TOLERANCE
				&&  fabs(m1[6] - m2[6]) < TOLERANCE
				&&  fabs(m1[7] - m2[7]) < TOLERANCE
				&&  fabs(m1[8] - m2[8]) < TOLERANCE);
		}
		friend bool operator!=(const mat3 &m1, const mat3 &m2) {
			return !(m1 == m2);
		}

		friend mat3 operator+(const mat3 &m1, const mat3 &m2) {
			return mat3(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2], 
				        m1[3] + m2[3], m1[4] + m2[4], m1[5] + m2[5],
				        m1[6] + m2[6], m1[7] + m2[7], m1[8] + m2[8]);
		}
		friend mat3 operator-(const mat3 &m1, const mat3 &m2) {
			return mat3(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2],
				        m1[3] - m2[3], m1[4] - m2[4], m1[5] - m2[5],
			        	m1[6] - m2[6], m1[7] - m2[7], m1[8] - m2[8]);
		}
		friend mat3 operator*(const mat3 &m, float scalar) {
			return mat3(m[0] * scalar, m[1] * scalar, m[2] * scalar, 
						m[3] * scalar, m[4] * scalar, m[5] * scalar,
						m[6] * scalar, m[7] * scalar, m[8] * scalar);
		}
		friend mat3 operator*(float scalar, const mat3 &m) {
			return mat3(m[0] * scalar, m[1] * scalar, m[2] * scalar,
						m[3] * scalar, m[4] * scalar, m[5] * scalar,
						m[6] * scalar, m[7] * scalar, m[8] * scalar);
		}
		friend mat3 operator*(const mat3 &m1, const mat3 &m2) {
			return mat3(m1[0] * m2[0] + m1[1] * m2[3] + m1[2] * m2[6], m1[0] * m2[1] + m1[1] * m2[4] + m1[2] * m2[7], m1[0] * m2[2] + m1[1] * m2[5] + m1[2] * m2[8],
				        m1[3] * m2[0] + m1[4] * m2[3] + m1[5] * m2[6], m1[3] * m2[1] + m1[4] * m2[4] + m1[5] * m2[7], m1[3] * m2[2] + m1[4] * m2[5] + m1[5] * m2[8],
				        m1[6] * m2[0] + m1[7] * m2[3] + m1[8] * m2[6], m1[6] * m2[1] + m1[7] * m2[4] + m1[8] * m2[7], m1[6] * m2[2] + m1[7] * m2[5] + m1[8] * m2[8]);
		}
		friend vec2 operator*(const mat3 &m, const vec2 &v) {

			return vec2(m[0] * v.x + m[1] * v.y + m[2],
						m[3] * v.x + m[4] * v.y + m[5]);
		}
		friend vec3 operator*(const mat3 &m, const vec3 &v) {
			return vec3(m[0] * v.x + m[1] * v.y + m[2] * v.z,
				        m[3] * v.x + m[4] * v.y + m[5] * v.z,
			         	m[6] * v.x + m[7] * v.y + m[8] * v.z);
		}


		mat3 &operator = (const mat3 &m);
		mat3 &operator += (const mat3 &m);
		mat3 &operator -= (const mat3 &m);
		mat3 &operator *= (float scalar);

		void Clean();
		void MakeZero();

		float Determinant();
		void Inverse();
		mat3 Inversed();
		void Transpose();
		mat3 Transposed();

		const float* Export();
	};

	struct mat4 {
		float data[16];

		float& mat4::operator[] (const int index) {
			return data[index];
		}
		const float& mat4::operator[] (const int index) const {
			return data[index];
		}

		mat4() {
			data[0]  = 0.0f; data[1]  = 0.0f; data[2]  = 0.0f; data[3]  = 0.0f;	
			data[4]  = 0.0f; data[5]  = 0.0f; data[6]  = 0.0f; data[7]  = 0.0f;	
			data[8]  = 0.0f; data[9]  = 0.0f; data[10] = 0.0f; data[11] = 0.0f;
			data[12] = 0.0f; data[13] = 0.0f; data[14] = 0.0f; data[15] = 0.0f;
		}

		mat4(float a, float b, float c,	float d, 
			 float e, float f, float g, float h, 
			 float i, float j, float k, float l,
			 float m, float n, float o, float p) {

			data[0]  = a; data[1]  = b; data[2]  = c; data[3]  = d;
			data[4]  = e; data[5]  = f; data[6]  = g; data[7]  = h;
			data[8]  = i; data[9]  = j; data[10] = k; data[11] = l;
			data[12] = m; data[13] = n; data[14] = o; data[15] = p;
		}
		mat4(const mat4 &m) {
			data[0]  = m[0];  data[1]  = m[1];   data[2]  = m[2];   data[3]  = m[3];
			data[4]  = m[4];  data[5]  = m[5];   data[6]  = m[6];   data[7]  = m[7];
			data[8]  = m[8];  data[9]  = m[9];   data[10] = m[10];  data[11] = m[11];
			data[12] = m[12]; data[13] = m[13];  data[14] = m[14];  data[15] = m[15];
		}

		friend std::ostream&  mat4::operator<<(std::ostream &os, mat4 &m) {
			m.Clean();
			os << '[' << m[0]  << ' ' << m[1]  << ' ' << m[2]  << ' ' << m[3]  << std::endl;
			os << ' ' << m[4]  << ' ' << m[5]  << ' ' << m[6]  << ' ' << m[7]  << std::endl;
			os << ' ' << m[8]  << ' ' << m[9]  << ' ' << m[10] << ' ' << m[11] << std::endl;
			os << ' ' << m[12] << ' ' << m[13] << ' ' << m[14] << ' ' << m[15] << ']';
			return os;
		}
		friend std::istream&  mat4::operator>>(std::istream &is, mat4 &m) {
			is >> m[0]  >> m[1]  >> m[2]  >> m[3];
			is >> m[4]  >> m[5]  >> m[6]  >> m[7];
			is >> m[8]  >> m[9]  >> m[10] >> m[11];
			is >> m[12] >> m[13] >> m[14] >> m[15];
			return is;
		}

		friend bool operator==(const mat4 &m1, const mat4 &m2) {
			return (fabs(m1[0] - m2[0]) < TOLERANCE
				&&  fabs(m1[1] - m2[1]) < TOLERANCE
				&&  fabs(m1[2] - m2[2]) < TOLERANCE
				&&  fabs(m1[3] - m2[3]) < TOLERANCE
				&&  fabs(m1[4] - m2[4]) < TOLERANCE
				&&  fabs(m1[5] - m2[5]) < TOLERANCE
				&&  fabs(m1[6] - m2[6]) < TOLERANCE
				&&  fabs(m1[7] - m2[7]) < TOLERANCE
				&&  fabs(m1[7] - m2[8]) < TOLERANCE
				&&  fabs(m1[7] - m2[9]) < TOLERANCE
				&&  fabs(m1[7] - m2[10]) < TOLERANCE
				&&  fabs(m1[7] - m2[11]) < TOLERANCE
				&&  fabs(m1[7] - m2[12]) < TOLERANCE
				&&  fabs(m1[7] - m2[13]) < TOLERANCE
				&&  fabs(m1[7] - m2[14]) < TOLERANCE
				&&  fabs(m1[8] - m2[15]) < TOLERANCE);
		}
		friend bool operator!=(const mat4 &m1, const mat4 &m2) {
			return !(m1 == m2);
		}

		friend mat4 operator+(const mat4 &m1, const mat4 &m2) {
			return mat4(m1[0]  + m2[0],  m1[1]  + m2[1],  m1[2]  + m2[2],  m1[3]  + m2[3],
						m1[4]  + m2[4],  m1[5]  + m2[5],  m1[6]  + m2[6],  m1[7]  + m2[7],
						m1[8]  + m2[8],  m1[9]  + m2[9],  m1[10] + m2[10], m1[11] + m2[11],
				        m1[12] + m2[12], m1[13] + m2[13], m1[14] + m2[14], m1[15] + m2[15]);
		}
		friend mat4 operator-(const mat4 &m1, const mat4 &m2) {
			return mat4(m1[0]  - m2[0],  m1[1]  - m2[1],  m1[2]  - m2[2],  m1[3]  - m2[3],
						m1[4]  - m2[4],  m1[5]  - m2[5],  m1[6]  - m2[6],  m1[7]  - m2[7],
						m1[8]  - m2[8],  m1[9]  - m2[9],  m1[10] - m2[10], m1[11] - m2[11],
						m1[12] - m2[12], m1[13] - m2[13], m1[14] - m2[14], m1[15] - m2[15]);
		}
		friend mat4 operator*(const mat4 &m, float scalar) {
			return mat4(m[0]  * scalar, m[1]  * scalar, m[2]  * scalar, m[3]  * scalar,
						m[4]  * scalar, m[5]  * scalar, m[6]  * scalar, m[7]  * scalar,
						m[8]  * scalar, m[9]  * scalar, m[10] * scalar, m[11] * scalar,
						m[12] * scalar, m[13] * scalar, m[14] * scalar, m[15] * scalar);
		}
		friend mat4 operator*(float scalar, const mat4 &m) {
			return mat4(m[0]  * scalar, m[1]  * scalar, m[2]  * scalar, m[3]  * scalar,
						m[4]  * scalar, m[5]  * scalar, m[6]  * scalar, m[7]  * scalar,
						m[8]  * scalar, m[9]  * scalar, m[10] * scalar, m[11] * scalar,
						m[12] * scalar, m[13] * scalar, m[14] * scalar, m[15] * scalar);
		}
		friend mat4 operator*(const mat4 &m1, const mat4 &m2) {
			return mat4	(m1[1]  * m2[4] + m1[2]  * m2[8] + m2[0] * m1[0]  + m2[12] * m1[3],  m2[1] * m1[0]  + m2[5] * m1[1]  + m2[9] * m1[2]  + m2[13] * m1[3],  m2[2] * m1[0]  + m2[6] * m1[1]  + m2[10] * m1[2]  + m2[14] * m1[3],  m2[3] * m1[0]  + m2[7] * m1[1]  + m2[11] * m1[2]  + m2[15] * m1[3],
					  	 m1[5]  * m2[4] + m1[6]  * m2[8] + m2[0] * m1[4]  + m2[12] * m1[7],  m2[1] * m1[4]  + m2[5] * m1[5]  + m2[9] * m1[6]  + m2[13] * m1[7],  m2[2] * m1[4]  + m2[6] * m1[5]  + m2[10] * m1[6]  + m2[14] * m1[7],  m2[3] * m1[4]  + m2[7] * m1[5]  + m2[11] * m1[6]  + m2[15] * m1[7],
						 m1[9]  * m2[4] + m1[10] * m2[8] + m2[0] * m1[8]  + m2[12] * m1[11], m2[1] * m1[8]  + m2[5] * m1[9]  + m2[9] * m1[10] + m2[13] * m1[11], m2[2] * m1[8]  + m2[6] * m1[9]  + m2[10] * m1[10] + m2[14] * m1[11], m2[3] * m1[8]  + m2[7] * m1[9]  + m2[11] * m1[10] + m2[15] * m1[11],
						 m1[13] * m2[4] + m1[14] * m2[8] + m2[0] * m1[12] + m2[12] * m1[15], m2[1] * m1[12] + m2[5] * m1[13] + m2[9] * m1[14] + m2[13] * m1[15], m2[2] * m1[12] + m2[6] * m1[13] + m2[10] * m1[14] + m2[14] * m1[15], m2[3] * m1[12] + m2[7] * m1[13] + m2[11] * m1[14] + m2[15] * m1[15]);
		}

		friend vec3 operator*(const mat4 &m, const vec3 &v) {
			return vec3(m[0] * v.x + m[1] * v.y + m[2]  * v.z + m[3],
						m[4] * v.x + m[5] * v.y + m[6]  * v.z + m[7],
						m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11]);
		}

		friend vec4 operator*(const mat4 &m, const vec4 &v) {
			return vec4(m[3]  * v.w + m[0]  * v.x + m[1]  * v.y + m[2]  * v.z,
						m[7]  * v.w + m[4]  * v.x + m[5]  * v.y + m[6]  * v.z,
						m[11] * v.w + m[8]  * v.x + m[9]  * v.y + m[10] * v.z,
						m[15] * v.w + m[12] * v.x + m[13] * v.y + m[14] * v.z);
		}


		mat4 &operator = (const mat4 &m);

		void Clean();
		void MakeZero();
		void Transpose();
		mat4 Transposed();

		const float* Export() const;

	};



	struct matFactory {

		//MAT2
		static mat2 Zero2();
		static mat2 Identity2();
		static mat3 Scale2(float sx, float sy);
		static mat3 Scale2(vec2 &v);
		static mat3 Translate2(float dx, float dy);
		static mat3 Translate2(vec2 &v);
		static mat3 Rotate2(float angle);
		static mat3 Mat2ToMat3(mat2 &m);


		//MAT3
		static mat3 Zero3();
		static mat3 Identity3();
		static mat4 Scale3(float sx, float sy, float sz);
		static mat4 Scale3(vec3 &v);
		static mat4 Translate3(float dx, float dy, float dz);
		static mat4 Translate3(vec3 &v);
		static mat4 Rotate3(vec3 &axe, float angle);
		static mat4 Shear3(vec3 &shx, vec3 &shy, vec3 &shz);

		static mat3 DualMatrix(vec3 &v);
		static mat3 sqrDualMatrix(vec3 &v);

		static mat4 PerspectiveProjection(float fovy, float aspect, float near, float far);
		static mat4 OrthogonalProjection(float left, float right, float bottom, float top, float near, float far);

		static mat3 MinorsMatrix(mat3 &m);
		static mat3 CoFactorsMatrix(mat3 &m);
		static mat3 AdjointMatrix(mat3 &m);
		static mat2 Mat3ToMat2(mat3 &m);
		static mat4 Mat3ToMat4(mat3 &m);

		//MAT4
		static mat4 Zero4();
		static mat4 Identity4();
		static mat3 Mat4ToMat3(mat4 &m);

	};
}