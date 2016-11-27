#include "stdafx.h"
#include "quaternion.h"

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

  float qThreshold = (float)1.0e-5;

quaternion::quaternion()
{
}

quaternion::quaternion(float _t, float _x, float _y, float _z)
{
	t = _t;
	x = _x;
	y = _y;
	z = _z;
}

quaternion quaternion::qFromAngleAxis(float theta, vector4d axis)
{
	vector4d axisn = axis.normalize();
	quaternion q;
	float a = theta * (float)DEGREES_TO_RADIANS;
	q.t = cosf(a / 2.0f);
	float s = sinf(a / 2.0f);
	q.x = axisn.getX() * s;
	q.y = axisn.getY() * s;
	q.z = axisn.getZ() * s;

	qClean(q);
	return qNormalize(q);
}

  void quaternion::qToAngleAxis(const quaternion& q, float& theta, vector4d& axis)
{
	quaternion qn = qNormalize(q);
	theta = 2.0f * acos(qn.t) * (float)RADIANS_TO_DEGREES;
	float s = sqrt(1.0f - qn.t*qn.t);
	if (s < qThreshold) {
		axis.setX(1.0f);
		axis.setY(0.0f);
		axis.setZ(0.0f);
		axis.setW(1.0f);
	}
	else {
		axis.setX(qn.x / s);
		axis.setY(qn.y / s);
		axis.setZ(qn.z / s);
		axis.setW(1.0f);
	}
}

  void quaternion::qClean(quaternion& q)
{
	if (fabs(q.t) < qThreshold) q.t = 0.0f;
	if (fabs(q.x) < qThreshold) q.x = 0.0f;
	if (fabs(q.y) < qThreshold) q.y = 0.0f;
	if (fabs(q.z) < qThreshold) q.z = 0.0f;
}

  float quaternion::qQuadrance(const  quaternion& q)
{
	 return q.t*q.t + q.x*q.x + q.y*q.y + q.z*q.z;
}

  float quaternion::qNorm(const  quaternion& q)
{
	return sqrt(qQuadrance(q));
}

  quaternion quaternion::qNormalize(const  quaternion& q)
{
	float s = 1 / qNorm(q);
	return qMultiply(q, s);
}

  quaternion quaternion::qConjugate(const  quaternion& q)
{
	quaternion qconj = { q.t, -q.x, -q.y, -q.z };
	return qconj;
}

  quaternion quaternion::qInverse(const  quaternion& q)
{
	return qMultiply(qConjugate(q), 1.0f / qQuadrance(q));
}

  quaternion quaternion::qAdd(const quaternion& q0, const  quaternion& q1)
{
	quaternion q;
	q.t = q0.t + q1.t;
	q.x = q0.x + q1.x;
	q.y = q0.y + q1.y;
	q.z = q0.z + q1.z;
	return q;
}

  quaternion quaternion::qMultiply( const quaternion& q,   float s)
{
	quaternion sq;
	sq.t = s * q.t;
	sq.x = s * q.x;
	sq.y = s * q.y;
	sq.z = s * q.z;
	return sq;
}

quaternion quaternion::qMultiply( const quaternion& q0,   quaternion& q1)
{
	quaternion q;
	q.t = q0.t * q1.t - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
	q.x = q0.t * q1.x + q0.x * q1.t + q0.y * q1.z - q0.z * q1.y;
	q.y = q0.t * q1.y + q0.y * q1.t + q0.z * q1.x - q0.x * q1.z;
	q.z = q0.t * q1.z + q0.z * q1.t + q0.x * q1.y - q0.y * q1.x;
	return q;
}

quaternion quaternion::operator *(quaternion& q2) {
	return qMultiply(*new quaternion(t,x,y,z),q2);
}

  void quaternion::qGLMatrix(const  quaternion& q, matrix4d& matrix)
{
	quaternion qn = qNormalize(q);

	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;



	float* m = matrix.getM();

	m[0] = 1.0f - 2.0f * (yy + zz);
	m[1] = 2.0f * (xy + zt);
	m[2] = 2.0f * (xz - yt);
	m[3] = 0.0f;

	m[4] = 2.0f * (xy - zt);
	m[5] = 1.0f - 2.0f * (xx + zz);
	m[6] = 2.0f * (yz + xt);
	m[7] = 0.0f;

	m[8] = 2.0f * (xz + yt);
	m[9] = 2.0f * (yz - xt);
	m[10] = 1.0f - 2.0f * (xx + yy);
	m[11] = 0.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
	
	matrix.setM(m);
	matrix = matrix.T();
	matrix.mClean();
}

  quaternion quaternion::qLerp(const quaternion& q0, const  quaternion& q1, float k)
{
	float cos_angle = q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	quaternion qi = qAdd(qMultiply(q0, k0), qMultiply(q1, k1));
	return qNormalize(qi);
}

  quaternion quaternion::qSlerp(const quaternion& q0, const  quaternion& q1, float k)
{
	float angle = acos(q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t);
	float k0 = sin((1 - k)*angle) / sin(angle);
	float k1 = sin(k*angle) / sin(angle);
	quaternion qi = qAdd(qMultiply(q0, k0), qMultiply(q1, k1));
	return qNormalize(qi);
}

  bool quaternion::qEqual(const  quaternion& q0, const   quaternion& q1)
{
	return (fabs(q0.t - q1.t) < qThreshold && fabs(q0.x - q1.x) < qThreshold &&
		fabs(q0.y - q1.y) < qThreshold && fabs(q0.z - q1.z) < qThreshold);
}

  void quaternion::qPrint(const  std::string& s, const  quaternion& q)
{
	std::cout << s << " = (" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
}

  void quaternion::qPrintAngleAxis(const  std::string& s, const  quaternion& q)
{
	std::cout << s << " = [" << std::endl;

	float thetaf;
	vector4d axis_f;
	qToAngleAxis(q, thetaf, axis_f);
	std::cout << "  angle = " << thetaf << std::endl;
	std::cout<<"  axis"<<axis_f;
	std::cout << "]" << std::endl;
}




quaternion::~quaternion()
{
}
