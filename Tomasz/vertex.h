#pragma once

#include "vector4d.h"

class vertex
{
	vector4d XYZW;
	vector4d RGBA;

public:
	vertex(vector4d, vector4d);
	vertex(GLfloat [], GLfloat []); 
	vector4d getXYZW();
	vector4d getRGBA();
	void setXYZW(GLfloat[]);
	void setRGBA(GLfloat[]);
};

