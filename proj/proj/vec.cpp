#include "vec.h"


namespace engine {

	// VEC2

	vec2 &vec2::operator=(const vec2 &v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	vec2 &vec2::operator+=(const vec2 &v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	vec2 &vec2::operator-=(const vec2 &v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	vec2 &vec2::operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}
	vec2 &vec2::operator/=(float scalar) {
		if (scalar != 0) {
			scalar = 1.0f / scalar;
			x *= scalar;
			y *= scalar;
		}
		return *this;
	}

	void vec2::Clean() {
		if (fabs(x) < TOLERANCE) x = 0;
		if (fabs(y) < TOLERANCE) y = 0;
	}
	void vec2::MakeZero() {
		Set(0.0f, 0.0f);
	}
	bool vec2::IsZero() {
		return (fabs(x) < TOLERANCE
			&& fabs(y) < TOLERANCE);
	}

	float vec2::sqrMagnitude() {
		return x*x + y*y;
	}
	float vec2::Magnitude() {
		return sqrt(sqrMagnitude());
	}

	void vec2::Normalize() {
		float magnitude = Magnitude();
		if (magnitude != 0) {
			magnitude = 1.0f / magnitude;

			x *= magnitude;
			y *= magnitude;
		}
	}
	vec2 vec2::Normalized() {
		float magnitude = Magnitude();
		magnitude = 1.0f / magnitude;
		if (magnitude != 0)
			return vec2(x * magnitude, y * magnitude);
		else return *this;
	}

	const float* vec2::Export() const {
		return &x;
	}


	// VEC3
	vec3 &vec3::operator=(const vec3 &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	vec3 &vec3::operator+=(const vec3 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	vec3 &vec3::operator-=(const vec3 &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	vec3 &vec3::operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	vec3 &vec3::operator/=(float scalar) {
		if (scalar != 0) {
			scalar = 1.0f / scalar;
			x *= scalar;
			y *= scalar;
			z *= scalar;
		}
		return *this;
	}

	void vec3::Clean() {
		if (fabs(x) < TOLERANCE) x = 0;
		if (fabs(y) < TOLERANCE) y = 0;
		if (fabs(z) < TOLERANCE) z = 0;
	}

	
	void vec3::MakeZero() {
		Set(0.0f, 0.0f, 0.0f);
	}
	bool vec3::IsZero() {
		return (   fabs(x) < TOLERANCE 
				&& fabs(y) < TOLERANCE
				&& fabs(z) < TOLERANCE);
	}

	float vec3::sqrMagnitude() {
		return x*x + y*y + z*z;
	}
	float vec3::Magnitude() {
		return sqrt(sqrMagnitude());
	}
	
	void vec3::Normalize() {
		float magnitude = Magnitude();
		if (magnitude != 0) {
			magnitude = 1.0f / magnitude;

			x *= magnitude;
			y *= magnitude;
			z *= magnitude;
		}
	}
	vec3 vec3::Normalized() {
		float magnitude = Magnitude();
		if (magnitude != 0) {
			magnitude = 1.0f / magnitude;
			return vec3(x * magnitude, y * magnitude, z * magnitude);
		}
		else return *this;
	}

	const float* vec3::Export() const {
		return &x;
	}

	// vec4
	vec4 &vec4::operator=(const vec4 &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}
	vec4 &vec4::operator+=(const vec4 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	vec4 &vec4::operator-=(const vec4 &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	vec4 &vec4::operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}
	vec4 &vec4::operator/=(float scalar) {
		if (scalar != 0) {
			scalar = 1.0f / scalar;
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
		}
		return *this;
	}

	void vec4::Clean() {
		if (fabs(x) < TOLERANCE) x = 0;
		if (fabs(y) < TOLERANCE) y = 0;
		if (fabs(z) < TOLERANCE) z = 0;
		if (fabs(w) < TOLERANCE) w = 0;
	}


	void vec4::MakeZero() {
		Set(0.0f, 0.0f, 0.0f, 0.0f);
	}
	bool vec4::IsZero() {
		return (fabs(x) < TOLERANCE
			&& fabs(y) < TOLERANCE
			&& fabs(z) < TOLERANCE
			&& fabs(w) < TOLERANCE);
	}

	float vec4::sqrMagnitude() {
		return x*x + y*y + z*z + w*w;
	}
	float vec4::Magnitude() {
		return sqrt(sqrMagnitude());
	}

	void vec4::Normalize() {
		float magnitude = Magnitude();
		if (magnitude != 0) {
			magnitude = 1.0f / magnitude;

			x *= magnitude;
			y *= magnitude;
			z *= magnitude;
			w *= magnitude;
		}
	}
	vec4 vec4::Normalized() {
		float magnitude = Magnitude();
		if (magnitude != 0) {
			magnitude = 1.0f / magnitude;
			return vec4(x * magnitude, y * magnitude, z * magnitude, w * magnitude);
		}
		else return *this;
	}

	const float* vec4::Export() const {
		return &x;
	}
}