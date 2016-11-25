#include "mat.h"

namespace engine {
	
	// **** MAT2 ****
	mat2 &mat2::operator = (const mat2 &m) {
		data[0] = m[0];
		data[1] = m[1];
		data[2] = m[2];
		data[3] = m[3];
		return *this;
	}
	mat2 &mat2::operator += (const mat2 &m) {
		data[0] += m[0];
		data[1] += m[1];
		data[2] += m[2];
		data[3] += m[3];
		return *this;
	}
	mat2 &mat2::operator -= (const mat2 &m) {
		data[0] -= m[0];
		data[1] -= m[1];
		data[2] -= m[2];
		data[3] -= m[3];
		return *this;
	}
	mat2 &mat2::operator *= (float scalar) {
		data[0] *= scalar;
		data[1] *= scalar;
		data[2] *= scalar;
		data[3] *= scalar;
		return *this;
	}

	void mat2::Clean() {
		if (fabs(data[0]) < TOLERANCE) data[0] = 0;
		if (fabs(data[1]) < TOLERANCE) data[1] = 0;
		if (fabs(data[2]) < TOLERANCE) data[2] = 0;
		if (fabs(data[3]) < TOLERANCE) data[3] = 0;
	}
	void mat2::MakeZero() {
		data[0] = 0;
		data[1] = 0;
		data[2] = 0;
		data[3] = 0;
	}

	float mat2::Determinant() {
		return data[0] * data[3] - data[1] * data[2];
	}
	void mat2::Inverse() {
		float factor = Determinant();
		float temp;
		if (factor != 0) {
			factor = 1 / Determinant();
			temp = data[0];
			data[0] = factor *  data[3];
			data[1] = factor * -data[1];
			data[2] = factor * -data[2];
			data[3] = factor *  temp;
		}
		else {
			std::cerr << "Mat2 Inverse: determinant == 0" << std::endl;
		}
	}
	mat2 mat2::Inversed() {
		float factor = Determinant();
		if (factor != 0) {
			factor = 1 / Determinant();
			return mat2(factor *  data[3], factor * -data[1],
				        factor * -data[2], factor *  data[0]);
		}
		else {
			std::cerr << "Mat2 Inverse: determinant == 0" << std::endl;
		}
	}

	void mat2::Transpose() {
		float temp = data[1];
		data[1] = data[2];
		data[2] = temp;
	}
	mat2 mat2::Transposed() {
		return mat2(data[0], data[2], data[1], data[3]);
	}

	const float* mat2::Export() {
		return &Transposed()[0];
	}


	// **** MAT3 **** 
	mat3 &mat3::operator = (const mat3 &m) {
		data[0] = m[0]; data[1] = m[1]; data[2] = m[2];
		data[3] = m[3]; data[4] = m[4]; data[5] = m[5];
		data[6] = m[6]; data[7] = m[7]; data[8] = m[8];
		return *this;
	}
	mat3 &mat3::operator += (const mat3 &m) {
		data[0] += m[0]; data[1] += m[1]; data[2] += m[2];
		data[3] += m[3]; data[4] += m[4]; data[5] += m[5];
		data[6] += m[6]; data[7] += m[7]; data[8] += m[8];
		return *this;
	}
	mat3 &mat3::operator -= (const mat3 &m) {
		data[0] -= m[0]; data[1] -= m[1]; data[2] -= m[2];
		data[3] -= m[3]; data[4] -= m[4]; data[5] -= m[5];
		data[6] -= m[6]; data[7] -= m[7]; data[8] -= m[8];
		return *this;
	}
	mat3 &mat3::operator *= (float scalar) {
		data[0] *= scalar; data[1] *= scalar; data[2] *= scalar;
		data[3] *= scalar; data[4] *= scalar; data[5] *= scalar;
		data[6] *= scalar; data[7] *= scalar; data[8] *= scalar;
		return *this;
	}
	

	void mat3::Clean() {
		if (fabs(data[0]) < TOLERANCE) data[0] = 0;
		if (fabs(data[1]) < TOLERANCE) data[1] = 0;
		if (fabs(data[2]) < TOLERANCE) data[2] = 0;
		if (fabs(data[3]) < TOLERANCE) data[3] = 0;
		if (fabs(data[4]) < TOLERANCE) data[4] = 0;
		if (fabs(data[5]) < TOLERANCE) data[5] = 0;
		if (fabs(data[6]) < TOLERANCE) data[6] = 0;
		if (fabs(data[7]) < TOLERANCE) data[7] = 0;
		if (fabs(data[8]) < TOLERANCE) data[8] = 0;
	}
	void mat3::MakeZero() {
		data[0] = 0.0f;	data[1] = 0.0f;	data[2] = 0.0f;
		data[3] = 0.0f;	data[4] = 0.0f;	data[5] = 0.0f;
		data[6] = 0.0f;	data[7] = 0.0f;	data[8] = 0.0f;
	}

	float mat3::Determinant() {
		return   data[0] * (data[4] * data[8] - data[5] * data[7])
			   - data[1] * (data[3] * data[8] - data[5] * data[6])
		       + data[2] * (data[3] * data[7] - data[4] * data[6]);
	}
	void mat3::Inverse() {
		float factor = Determinant();
		if (factor != 0) {
			*this = (1 / factor) * matFactory::AdjointMatrix(*this);
		}
		else {
			std::cerr << "Mat3 Inverse: determinant == 0" << std::endl;
		}
	}
	mat3 mat3::Inversed() {
		float factor = Determinant();
		if (factor != 0) {			
			return (1 / factor) * matFactory::AdjointMatrix(*this);
		}
		else {
			std::cerr << "Mat3 Inverse: determinant == 0" << std::endl;
		}
	}

	void mat3::Transpose() {
		float temp = data[1];
		data[1] = data[3];
		data[3] = temp;
		temp = data[2];
		data[2] = data[6];
		data[6] = temp;
		temp = data[5];
		data[5] = data[7];
		data[7] = temp;
	}
	mat3 mat3::Transposed() {
		return mat3(data[0], data[3], data[6], 
			        data[1], data[4], data[7], 
		        	data[2], data[5], data[8]);
	}

	const float* mat3::Export() {
		return &Transposed()[0];
	}

	// **** MAT4 ****
	mat4 &mat4::operator = (const mat4 &m) {
		data[0] = m[0];  data[1] = m[1];   data[2] = m[2];   data[3] = m[3];
		data[4] = m[4];  data[5] = m[5];   data[6] = m[6];   data[7] = m[7];
		data[8] = m[8];  data[9] = m[9];   data[10] = m[10];  data[11] = m[11];
		data[12] = m[12]; data[13] = m[13];  data[14] = m[14];  data[15] = m[15];
		return *this;
	}
	


	void mat4::Clean() {
		if (fabs(data[0]) < TOLERANCE) data[0] = 0;
		if (fabs(data[1]) < TOLERANCE) data[1] = 0;
		if (fabs(data[2]) < TOLERANCE) data[2] = 0;
		if (fabs(data[3]) < TOLERANCE) data[3] = 0;
		if (fabs(data[4]) < TOLERANCE) data[4] = 0;
		if (fabs(data[5]) < TOLERANCE) data[5] = 0;
		if (fabs(data[6]) < TOLERANCE) data[6] = 0;
		if (fabs(data[7]) < TOLERANCE) data[7] = 0;
		if (fabs(data[8]) < TOLERANCE) data[8] = 0;
		if (fabs(data[9]) < TOLERANCE) data[9] = 0;
		if (fabs(data[10]) < TOLERANCE) data[10] = 0;
		if (fabs(data[11]) < TOLERANCE) data[11] = 0;
		if (fabs(data[12]) < TOLERANCE) data[12] = 0;
		if (fabs(data[13]) < TOLERANCE) data[13] = 0;
		if (fabs(data[14]) < TOLERANCE) data[14] = 0;
		if (fabs(data[15]) < TOLERANCE) data[15] = 0;
	}
	void mat4::MakeZero() {
		data[0]  = 0.0f; data[1]  = 0.0f; data[2]  = 0.0f; data[3]  = 0.0f;
		data[4]  = 0.0f; data[5]  = 0.0f; data[6]  = 0.0f; data[7]  = 0.0f;
		data[8]  = 0.0f; data[9]  = 0.0f; data[10] = 0.0f; data[11] = 0.0f;
		data[12] = 0.0f; data[13] = 0.0f; data[14] = 0.0f; data[15] = 0.0f;
	}

	void mat4::Transpose() {
		float temp = data[1];
		data[1] = data[4];
		data[4] = temp;
		temp = data[2];
		data[2] = data[8];
		data[8] = temp;
		temp = data[3];
		data[3] = data[12];
		data[12] = temp;
		temp = data[6];
		data[6] = data[9];
		data[9] = temp;
		temp = data[7];
		data[7] = data[13];
		data[13] = temp;
		temp = data[11];
		data[11] = data[14];
		data[14] = temp;
	}
	mat4 mat4::Transposed() {
		return mat4(data[0], data[4],  data[8], data[12],
					data[1], data[5],  data[9], data[13],
					data[2], data[6], data[10], data[14],
					data[3], data[7], data[11], data[15]);
	}

	const float* mat4::Export() const{
		return &data[0];
	}

	

	// **** MAT FACTORY ****
	//MAT2
	mat2 matFactory::Zero2() { return mat2(); }
	mat2 matFactory::Identity2() { 
		return mat2(1, 0, 
			        0, 1);  
	}
	mat3 matFactory::Scale2(float sx, float sy) { 
		return mat3(sx,  0,  0, 
			         0, sy,  0, 
			         0,  0,  1);  
	}
	mat3 matFactory::Scale2(vec2 &v) {
		return mat3(v.x,   0,  0,
					  0, v.y,  0,
					  0,   0,  1);
	
	}
	mat3 matFactory::Translate2(float dx, float dy) { 
		return mat3(1, 0, dx, 
					0, 1, dy, 
					0, 0, 1); 
	}
	mat3 matFactory::Translate2(vec2 &v) {
		return mat3(1,  0, v.x,
					0,  1, v.y,
				    0,  0,   1);

	}
	mat3 matFactory::Rotate2(float angle) {
		// RECEIVES DEGREES CONVERTS TO RADIANS
		angle = (angle * PI) / 180;
		return mat3(cos(angle), -sin(angle),  0,
			        sin(angle),   cos(angle), 0,
					0,           0,           1); 
	}

	mat3 matFactory::Mat2ToMat3(mat2 &m) {
		return mat3(m[0], m[1],  0,
					m[2], m[3],  0,
					   0,    0,  1);
	}


	//MAT3
	mat3 matFactory::Zero3() { return mat3(); }
	mat3 matFactory::Identity3() { 
		return mat3(1, 0, 0, 
					0, 1, 0, 
					0, 0, 1); 
	}
	mat4 matFactory::Scale3(float sx, float sy, float sz) {
		return mat4(sx,   0,   0,   0,
			         0,  sy,   0,   0,
			         0,   0,  sz,   0,
			         0,   0,   0,   1);
	}
	mat4 matFactory::Scale3(vec3 &v) {
		return mat4(v.x,  0,   0,  0,
					0,  v.y,   0,  0,
					0,   0,  v.z,  0,
					0,   0,    0,  1);
	}
	mat4 matFactory::Translate3(float dx, float dy, float dz) {
		return mat4(1,  0,  0,  dx,
					0,  1,  0,  dy,
					0,  0,  1,  dz,
					0,  0,  0,   1);
	}
	mat4 matFactory::Translate3(vec3 &v) {
		return mat4(1,  0,  0, v.x,
					0,  1,  0, v.y,
					0,  0,  1, v.z,
					0,  0,  0,   1);
	}
	mat4 matFactory::Rotate3(vec3 &axe, float angle) {
		// RECEIVES DEGREES CONVERTS TO RADIANS
		angle = (angle * PI) / 180;
		vec3 v = axe.Normalized();
		mat3 m = Identity3() + sin(angle) * DualMatrix(v) + (1 - cos(angle)) * sqrDualMatrix(v);
		return Mat3ToMat4(m);
	}

	mat4 matFactory::Shear3(vec3 &shx, vec3 &shy, vec3 &shz) {
		return mat4(    1,  shx.y,  shx.z,   0,
					shy.x,      1,  shy.z,   0,
					shz.x,  shz.y,      1,   0,
					   0,       0,      0,   1);
	}


	mat3 matFactory::DualMatrix(vec3 &v) {
		return mat3(   0,  -v.z,    v.y,
			         v.z,     0,   -v.x,
			        -v.y,   v.x,      0);
	}
	mat3 matFactory::sqrDualMatrix(vec3 &v) {
		return mat3( -(v.z*v.z) - (v.y*v.y),                v.x * v.y,             v.x * v.z,
			                      v.x * v.y,   -(v.z*v.z) - (v.x*v.x),             v.y * v.z,
					              v.x * v.z,                v.y * v.z, -(v.y*v.y) - (v.x*v.x));
	}


	mat4 matFactory::PerspectiveProjection(float fovy, float aspect, float near, float far) {
		// CONVERT FOV FROM DEGREES TO RADIANS
		float d = 1 / tan(((fovy * PI) / 180) / 2);

		return mat4(d / aspect, 0, 0, 0,
			0, d, 0, 0,
			0, 0, (far + near) / (near - far), (2 * far*near) / (near - far),
			0, 0, -1, 0);
	}
	mat4 matFactory::OrthogonalProjection(float left, float right, float bottom, float top, float near, float far) {
		return mat4(2 / (right - left), 0, 0, -(right + left) / (right - left),
			0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
			0, 0, -2 / (far - near), -(far + near) / (far - near),
			0, 0, 0, 1);
	}


	mat3 matFactory::MinorsMatrix(mat3 &m) {
			
		return mat3(m[4] * m[8] - m[5] * m[7], m[3] * m[8] - m[5] * m[6], m[3] * m[7] - m[4] * m[6],
			        m[1] * m[8] - m[2] * m[7], m[0] * m[8] - m[2] * m[6], m[0] * m[7] - m[1] * m[6],
			        m[1] * m[5] - m[2] * m[4], m[0] * m[5] - m[2] * m[3], m[0] * m[4] - m[1] * m[3]);
	}

	mat3 matFactory::CoFactorsMatrix(mat3 &m) {
		m = MinorsMatrix(m);
		return mat3( m[0], -m[1],  m[2],
					-m[3],  m[4], -m[5],
					 m[6], -m[7],  m[8]);
	}

	mat3 matFactory::AdjointMatrix(mat3 &m) {
		m = CoFactorsMatrix(m);
		return mat3(m.Transposed());
	}

	mat2 matFactory::Mat3ToMat2(mat3 &m) {
		return mat2(m[0], m[1],
					m[3], m[4]);
	}

	mat4 matFactory::Mat3ToMat4(mat3 &m) {
		return mat4(m[0], m[1], m[2], 0,
					m[3], m[4], m[5], 0,
					m[6], m[7], m[8], 0,
					   0,    0,    0, 1);
	}

	//MAT4
	mat4 matFactory::Zero4() { return mat4(); }
	mat4 matFactory::Identity4() {
		return mat4(1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);
	}
	mat3 matFactory::Mat4ToMat3(mat4 &m) {
		return mat3(m[0], m[1], m[2],
					m[4], m[5], m[6],
					m[8], m[9], m[10]);
	}


}