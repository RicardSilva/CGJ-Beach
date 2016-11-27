#pragma once

#include "matrix4d.h"

class quaternion
{
public:
	float t, x, y, z;

	quaternion();
	quaternion(float, float, float, float);
	
	 float qThreshold = (float)1.0e-5;

	quaternion qFromAngleAxis(float theta, vector4d axis);
	  void qToAngleAxis(const quaternion& q, float& theta, vector4d& axis);
	  void qClean(quaternion& q);
	  float qQuadrance(const quaternion& q);
	  float qNorm(const quaternion& q);
	  quaternion qNormalize(const quaternion& q);
	  quaternion qConjugate(const quaternion& q);
	  quaternion qInverse(const quaternion& q);
	  quaternion qMultiply(const quaternion& q,   float s);
	quaternion qMultiply(const quaternion& q0,   quaternion& q1);
	  void qGLMatrix(const quaternion& q, matrix4d& matrix);
	  quaternion qLerp(const quaternion& q0, const  quaternion& q1, float k);
	  quaternion qSlerp(const quaternion& q0, const quaternion& q1, float k);
	  bool qEqual( const quaternion& q0, const quaternion& q1);
	  void qPrint( const std::string& s, const quaternion& q);
	  void qPrintAngleAxis(const  std::string& s, const quaternion& q);
	  quaternion qAdd(const quaternion& q0, const  quaternion& q1);
	  quaternion quaternion::operator *( quaternion &q2);

	~quaternion();
};

///////////////////////////////////////////////////////////////////////