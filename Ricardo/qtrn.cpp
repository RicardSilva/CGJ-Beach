#include "qtrn.h"

namespace engine {

	qtrn &qtrn::operator=(const qtrn &q) {
		t = q.t;
		x = q.x;
		y = q.y;
		z = q.z;
		return *this;
	}

	const qtrn qtrn::qFromAngleAxis(float theta, vec4 axis)
	{
		vec4 axisn = axis.Normalized();

		qtrn q;
		float a = theta * (float)DEGREES_TO_RADIANS;
		q.t = cos(a / 2.0f);
		float s = sin(a / 2.0f);
		q.x = axisn.x * s;
		q.y = axisn.y * s;
		q.z = axisn.z * s;

		qClean(q);
		return qNormalize(q);
	}

	const void qtrn::qToAngleAxis(const qtrn& q, float& theta, vec4& axis)
	{
		qtrn qn = qNormalize(q);
		theta = 2.0f * acos(qn.t) * (float)RADIANS_TO_DEGREES;
		float s = sqrt(1.0f - qn.t*qn.t);
		if (s < qThreshold) {
			axis.x = 1.0f;
			axis.y = 0.0f;
			axis.z = 0.0f;
			axis.w = 1.0f;
		}
		else {
			axis.x = qn.x / s;
			axis.y = qn.y / s;
			axis.z = qn.z / s;
			axis.w = 1.0f;
		}
	}

	

	


}