#pragma once
#include "vec.h"
#include "mat.h"
#include <string>

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913
#define qThreshold 1.0e-5


namespace engine {

	struct qtrn {
		float t, x, y, z;


		qtrn() : t(0.0f), x(0.0f), y(0.0f), z(0.0f) {}
		qtrn(float T, float X, float Y, float Z) : t(T), x(X), y(Y), z(Z) {}
		qtrn(const qtrn &q) : t(q.t), x(q.x), y(q.y), z(q.z) {}

		qtrn &operator = (const qtrn &q);

		//const float qThreshold = (float)1.0e-5;

		static const qtrn qFromAngleAxis(float theta, vec4 axis);
		static const void qToAngleAxis(const qtrn& q, float& theta, vec4& axis);
		friend const void qClean(qtrn& q) 
		{
			if (fabs(q.t) < qThreshold) q.t = 0.0f;
			if (fabs(q.x) < qThreshold) q.x = 0.0f;
			if (fabs(q.y) < qThreshold) q.y = 0.0f;
			if (fabs(q.z) < qThreshold) q.z = 0.0f;
		}
		friend const float qQuadrance(const qtrn& q)
		{
			return q.t*q.t + q.x*q.x + q.y*q.y + q.z*q.z;
		}
		friend const float qNorm(const qtrn& q) 
		{
			return sqrt(qQuadrance(q));
		}
		friend const qtrn qNormalize(const qtrn& q)
		{
			float s = 1 / qNorm(q);
			return qMultiply(q, s);
		}
		friend const qtrn qConjugate(const qtrn& q)
		{
			qtrn qconj = qtrn(q.t, -q.x, -q.y, -q.z);
			return qconj;
		}
		friend const qtrn qInverse(const qtrn& q)
		{
			return qMultiply(qConjugate(q), 1.0f / qQuadrance(q));
		}

		friend const qtrn qAdd(const qtrn& q0, const qtrn& q1) 
		{
			qtrn q;
			q.t = q0.t + q1.t;
			q.x = q0.x + q1.x;
			q.y = q0.y + q1.y;
			q.z = q0.z + q1.z;
			return q;
		}
		friend const qtrn operator+(const qtrn& q0, const qtrn& q1) {
			qtrn q;
			q.t = q0.t + q1.t;
			q.x = q0.x + q1.x;
			q.y = q0.y + q1.y;
			q.z = q0.z + q1.z;
			return q;
		}
		friend const qtrn qMultiply(const qtrn& q, const float s) {
			
			qtrn sq;
			sq.t = s * q.t;
			sq.x = s * q.x;
			sq.y = s * q.y;
			sq.z = s * q.z;
			return sq;
			
		}
		friend const qtrn qMultiply(const qtrn& q0, const qtrn& q1)
		{
			qtrn q;
			q.t = q0.t * q1.t - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
			q.x = q0.t * q1.x + q0.x * q1.t + q0.y * q1.z - q0.z * q1.y;
			q.y = q0.t * q1.y + q0.y * q1.t + q0.z * q1.x - q0.x * q1.z;
			q.z = q0.t * q1.z + q0.z * q1.t + q0.x * q1.y - q0.y * q1.x;
			return q;
		}
		friend const qtrn operator*(const qtrn& q, const float s) {
			qtrn sq;
			sq.t = s * q.t;
			sq.x = s * q.x;
			sq.y = s * q.y;
			sq.z = s * q.z;
			return sq;
		}
		friend const qtrn operator*(const qtrn& q0, const qtrn& q1) {
			qtrn q;
			q.t = q0.t * q1.t - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
			q.x = q0.t * q1.x + q0.x * q1.t + q0.y * q1.z - q0.z * q1.y;
			q.y = q0.t * q1.y + q0.y * q1.t + q0.z * q1.x - q0.x * q1.z;
			q.z = q0.t * q1.z + q0.z * q1.t + q0.x * q1.y - q0.y * q1.x;
			return q;
		}
		friend const mat4 qGLMatrix(const qtrn& q) {
			mat4 matrix = mat4();
			qtrn qn = qNormalize(q);

			float xx = qn.x * qn.x;
			float xy = qn.x * qn.y;
			float xz = qn.x * qn.z;
			float xt = qn.x * qn.t;
			float yy = qn.y * qn.y;
			float yz = qn.y * qn.z;
			float yt = qn.y * qn.t;
			float zz = qn.z * qn.z;
			float zt = qn.z * qn.t;

			matrix[0] = 1.0f - 2.0f * (yy + zz);
			matrix[1] = 2.0f * (xy + zt);
			matrix[2] = 2.0f * (xz - yt);
			matrix[3] = 0.0f;

			matrix[4] = 2.0f * (xy - zt);
			matrix[5] = 1.0f - 2.0f * (xx + zz);
			matrix[6] = 2.0f * (yz + xt);
			matrix[7] = 0.0f;

			matrix[8] = 2.0f * (xz + yt);
			matrix[9] = 2.0f * (yz - xt);
			matrix[10] = 1.0f - 2.0f * (xx + yy);
			matrix[11] = 0.0f;

			matrix[12] = 0.0f;
			matrix[13] = 0.0f;
			matrix[14] = 0.0f;
			matrix[15] = 1.0f;

			matrix.Clean();
			return matrix;
		}
		friend const qtrn qLerp(const qtrn& q0, const qtrn& q1, float k)
		{
			float cos_angle = q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t;
			float k0 = 1.0f - k;
			float k1 = (cos_angle > 0) ? k : -k;
			qtrn qi = qAdd(qMultiply(q0, k0), qMultiply(q1, k1));
			return qNormalize(qi);
		}
		friend const qtrn qSlerp(const qtrn& q0, const qtrn& q1, float k) 
		{
			float angle = acos(q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t);
			float k0 = sin((1 - k)*angle) / sin(angle);
			float k1 = sin(k*angle) / sin(angle);
			qtrn qi = qAdd(qMultiply(q0, k0), qMultiply(q1, k1));
			return qNormalize(qi);
		}
		friend const bool qEqual(const qtrn& q0, const qtrn& q1) {
			return (fabs(q0.t - q1.t) < qThreshold && fabs(q0.x - q1.x) < qThreshold &&
				fabs(q0.y - q1.y) < qThreshold && fabs(q0.z - q1.z) < qThreshold);
		}

		friend const void qPrint(const std::string& s, const qtrn& q)
		{
			std::cout << s << " = (" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
		}
		friend const void qPrintAngleAxis(const std::string& s, const qtrn& q)
		{
			std::cout << s << " = [" << std::endl;

			float thetaf;
			vec4 axis_f;
			qToAngleAxis(q, thetaf, axis_f);
			std::cout << "  angle = " << thetaf << std::endl;
			vPrint("  axis", axis_f);
			std::cout << "]" << std::endl;
		}

		static const void vPrint(const std::string s, const vec4& v)
		{
			std::cout << s << " = (" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" << std::endl;
		}
	};

	
}