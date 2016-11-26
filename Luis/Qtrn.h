#ifndef Qtrn_H
#define Qtrn_H

#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>

#include "Vector4.h"
#include "Matrix4x4.h"

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

class Qtrn {
public:
	float _t;
	float _x;
	float _y;
	float _z;
	Qtrn() : _x(0), _y(0), _z(0), _t(0) {}
	Qtrn(float k) : _x(k), _y(k), _z(k), _t(k) {}
	Qtrn(float x, float y, float z, float t) : _x(x), _y(y), _z(z), _t(t) {}
	Qtrn(const Qtrn& Qtrn) : _x(Qtrn._x), _y(Qtrn._y), _z(Qtrn._z), _t(Qtrn._t) {}

};

const Qtrn qFromAngleAxis(float theta, Vector4 axis);
const void qToAngleAxis(const Qtrn& q, float& theta, Vector4& axis);
const void qClean(Qtrn& q);
const float qQuadrance(const Qtrn& q);
const float qNorm(const Qtrn& q);
const Qtrn qNormalize(const Qtrn& q);
const Qtrn qConjugate(const Qtrn& q);
const Qtrn qInverse(const Qtrn& q);
const Qtrn qMultiply(const Qtrn& q, const float s);
const Qtrn qMultiply(const Qtrn& q0, const Qtrn& q1);
const void qGLMatrix(const Qtrn& q, Matrix4x4 matrix);
const Qtrn qLerp(const Qtrn& q0, const Qtrn& q1, float k);
const Qtrn qSlerp(const Qtrn& q0, const Qtrn& q1, float k);
const bool qEqual(const Qtrn& q0, const Qtrn& q1);
const void qPrint(const std::string& s, const Qtrn& q);
const void qPrintAngleAxis(const std::string& s, const Qtrn& q);


#endif