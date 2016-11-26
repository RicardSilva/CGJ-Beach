#include "Qtrn.h"

const Qtrn qFromAngleAxis(float theta, Vector4 axis)
{
	Vector4 axisn = axis.normalize();

	Qtrn q;
	float a = theta * (float)DEGREES_TO_RADIANS;
	q._t = cos(a / 2.0f);
	float s = sin(a / 2.0f);
	q._x = axisn.getX() * s;
	q._y = axisn.getY() * s;
	q._z = axisn.getZ() * s;

	qClean(q);
	return qNormalize(q);
}

const void qToAngleAxis(const Qtrn& q, float& theta, Vector4& axis)
{
	Qtrn qn = qNormalize(q);
	theta = 2.0f * acos(qn._t) * (float)RADIANS_TO_DEGREES;
	float s = sqrt(1.0f - qn._t*qn._t);
	if (s < CLEANPREC) {
		axis.set(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		axis.set(qn._x / s, qn._y / s, qn._z / s, 1.0f);
	}
}

const void qClean(Qtrn& q)
{
	if (fabs(q._t) < CLEANPREC) q._t = 0.0f;
	if (fabs(q._x) < CLEANPREC) q._x = 0.0f;
	if (fabs(q._y) < CLEANPREC) q._y = 0.0f;
	if (fabs(q._z) < CLEANPREC) q._z = 0.0f;
}

const float qQuadrance(const Qtrn& q)
{
	return q._t*q._t + q._x*q._x + q._y*q._y + q._z*q._z;
}

const float qNorm(const Qtrn& q)
{
	return sqrt(qQuadrance(q));
}

const Qtrn qNormalize(const Qtrn& q)
{
	float s = 1 / qNorm(q);
	return qMultiply(q, s);
}

const Qtrn qConjugate(const Qtrn& q)
{
	Qtrn qconj = { q._t, -q._x, -q._y, -q._z };
	return qconj;
}

const Qtrn qInverse(const Qtrn& q)
{
	return qMultiply(qConjugate(q), 1.0f / qQuadrance(q));
}

const Qtrn qAdd(const Qtrn& q0, const Qtrn& q1)
{
	Qtrn q;
	q._t = q0._t + q1._t;
	q._x = q0._x + q1._x;
	q._y = q0._y + q1._y;
	q._z = q0._z + q1._z;
	return q;
}

const Qtrn qMultiply(const Qtrn& q, const float s)
{
	Qtrn sq;
	sq._t = s * q._t;
	sq._x = s * q._x;
	sq._y = s * q._y;
	sq._z = s * q._z;
	return sq;
}

const Qtrn qMultiply(const Qtrn& q0, const Qtrn& q1)
{
	Qtrn q;
	q._t = q0._t * q1._t - q0._x * q1._x - q0._y * q1._y - q0._z * q1._z;
	q._x = q0._t * q1._x + q0._x * q1._t + q0._y * q1._z - q0._z * q1._y;
	q._y = q0._t * q1._y + q0._y * q1._t + q0._z * q1._x - q0._x * q1._z;
	q._z = q0._t * q1._z + q0._z * q1._t + q0._x * q1._y - q0._y * q1._x;
	return q;
}

const void qGLMatrix(const Qtrn& q, Matrix4x4 matrix)
{
	Qtrn qn = qNormalize(q);

	float xx = qn._x * qn._x;
	float xy = qn._x * qn._y;
	float xz = qn._x * qn._z;
	float xt = qn._x * qn._t;
	float yy = qn._y * qn._y;
	float yz = qn._y * qn._z;
	float yt = qn._y * qn._t;
	float zz = qn._z * qn._z;
	float zt = qn._z * qn._t;

	matrix.setByIndex(0, 1.0f - 2.0f * (yy + zz));
	matrix.setByIndex(1, 2.0f * (xy + zt));
	matrix.setByIndex(2, 2.0f * (xz - yt));
	matrix.setByIndex(3, 0.0f);

	matrix.setByIndex(4, 2.0f * (xy - zt));
	matrix.setByIndex(5, 1.0f - 2.0f * (xx + zz));
	matrix.setByIndex(6, 2.0f * (yz + xt));
	matrix.setByIndex(7, 0.0f);

	matrix.setByIndex(8, 2.0f * (xz + yt));
	matrix.setByIndex(9, 2.0f * (yz - xt));
	matrix.setByIndex(10, 1.0f - 2.0f * (xx + yy));
	matrix.setByIndex(11, 0.0f);

	matrix.setByIndex(12, 0.0f);
	matrix.setByIndex(13, 0.0f);
	matrix.setByIndex(14, 0.0f);
	matrix.setByIndex(15, 1.0f);

	matrix.clean();
}

const Qtrn qLerp(const Qtrn& q0, const Qtrn& q1, float k)
{
	float cos_angle = q0._x*q1._x + q0._y*q1._y + q0._z*q1._z + q0._t*q1._t;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	Qtrn qi = qAdd(qMultiply(q0, k0), qMultiply(q1, k1));
	return qNormalize(qi);
}

const Qtrn qSlerp(const Qtrn& q0, const Qtrn& q1, float k)
{
	float angle = acos(q0._x*q1._x + q0._y*q1._y + q0._z*q1._z + q0._t*q1._t);
	float k0 = sin((1 - k)*angle) / sin(angle);
	float k1 = sin(k*angle) / sin(angle);
	Qtrn qi = qAdd(qMultiply(q0, k0), qMultiply(q1, k1));
	return qNormalize(qi);
}

const bool qEqual(const Qtrn& q0, const Qtrn& q1)
{
	return (fabs(q0._t - q1._t) < CLEANPREC && fabs(q0._x - q1._x) < CLEANPREC &&
		fabs(q0._y - q1._y) < CLEANPREC && fabs(q0._z - q1._z) < CLEANPREC);
}

const void qPrint(const std::string& s, const Qtrn& q)
{
	std::cout << s << " = (" << q._t << ", " << q._x << ", " << q._y << ", " << q._z << ")" << std::endl;
}

const void qPrintAngleAxis(const std::string& s, const Qtrn& q)
{
	std::cout << s << " = [" << std::endl;

	float thetaf;
	Vector4 axis_f;
	qToAngleAxis(q, thetaf, axis_f);
	std::cout << "  angle = " << thetaf << std::endl;
	std::cout << "  axis" << axis_f << std::endl;
	std::cout << "]" << std::endl;
}