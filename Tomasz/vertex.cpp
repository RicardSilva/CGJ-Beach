#include "stdafx.h"
#include "vertex.h"


vertex::vertex(GLfloat xyzw[], GLfloat rgba[])
{
	XYZW = *new vector4d(xyzw);
	RGBA = *new vector4d(rgba);
}

vertex::vertex(vector4d xyzw, vector4d rgba)
{
	XYZW = xyzw;
	RGBA = rgba;
}

vector4d vertex::getXYZW() {
	return XYZW;
}

vector4d vertex::getRGBA() {
	return RGBA;
}

void vertex::setXYZW(GLfloat xyzw[])
{
	XYZW = *new vector4d(xyzw);
}

void vertex::setRGBA(GLfloat rgba[])
{
	RGBA = *new vector4d(rgba);
}
