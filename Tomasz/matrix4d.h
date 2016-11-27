//
//  matrix4d.hpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 12/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//

#ifndef matrix4d_h
#define matrix4d_h

#include <stdio.h>
#include "matrix3d.h"
#include "vector4d.h"

class matrix4d{
    float M[16];
public:
    matrix4d();
    matrix4d(const float[]);
    matrix4d(const float,const float,const float,const float,const float,const float,const float,const float,const float,const float,const float,const float,const float,const float,const float,const float);
    matrix4d(const matrix3d);
	matrix4d(const vector4d, const vector4d, const vector4d, const vector4d);
    float* getM();
    void setM(const float *);
    matrix4d operator +(const matrix4d);
    matrix4d operator -(const matrix4d);
    matrix4d operator *(const float);
    bool operator ==(const matrix4d);
    matrix4d T();
    float det();
    matrix4d inv();
	const void mClean();
	matrix4d operator *(const matrix4d);
	vector4d operator *(vector4d);
    friend std::ostream& operator <<(std::ostream&, const matrix4d);
    friend std::istream& operator >>(std::istream&, matrix4d);
    void test();

};

#endif /* matrix4d_hpp */
