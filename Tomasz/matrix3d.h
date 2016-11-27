//
//  matrix3d.hpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 12/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//

#ifndef matrix3d_h
#define matrix3d_h

#include <stdio.h>
#include <iostream>
#include "matrix2d.h"
#include "vector3d.h"

class matrix3d{
    float M[9];
public:
    matrix3d();
    matrix3d(const float[]);
    matrix3d(const float,const float,const float,const float,const float,const float,const float,const float,const float);
    matrix3d(matrix2d); 
	matrix3d(vector3d, vector3d, vector3d);
    float* getM();
    void setM(const float *);
    matrix3d operator +(const matrix3d);
    matrix3d operator -(const matrix3d);
    matrix3d operator *(const float);
    bool operator ==(const matrix3d);
    matrix3d T();
    float det();
    matrix3d inv();
    matrix3d operator *(const matrix3d);
    vector3d operator *(vector3d);
    friend std::ostream& operator <<(std::ostream&, const matrix3d);
    friend std::istream& operator >>(std::istream&, matrix3d);
    void test();
};


#endif /* matrix3d_hpp */
