//
//  matrix2d.hpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 12/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//

#ifndef matrix2d_h
#define matrix2d_h

#include <stdio.h>
#include <iostream>
#include "vector2d.h"

class matrix2d{
    float M[4];
public:
    matrix2d();
    matrix2d(const float[]);
    matrix2d(const float,const float,const float,const float);
    float* getM();
    void setM(const float *);
    matrix2d operator +(const matrix2d);
    matrix2d operator -(const matrix2d);
    matrix2d operator *(const float);
    bool operator ==(const matrix2d);
    matrix2d T();
    float det();
    matrix2d inv();
    matrix2d operator *(const matrix2d);
    vector2d operator *(vector2d);
    static void test();
    friend std::ostream& operator <<(std::ostream&, const matrix2d);
    friend std::istream& operator >>(std::istream&, matrix2d);

};


#endif /* matrix2d_hpp */
