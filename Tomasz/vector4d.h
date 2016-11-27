//
//  vector4d.hpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 12/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//

#ifndef vector4d_h
#define vector4d_h

#include "vector3d.h"

class vector4d{
    float x;
    float y;
    float z;
    float w;
public:
	vector4d();
    vector4d(float,float,float,float);
    vector4d(vector3d);
    vector4d(vector3d,float);
	vector4d(GLfloat*);
    float getX();
    float getY();
    float getZ();
    float getW();
    void setX(float);
    void setY(float);
    void setZ(float);
    void setW(float);
    vector4d operator +(vector4d);
    vector4d operator -(vector4d);
    vector4d operator *(float);
    bool operator ==(vector4d);
    float norm();
    vector4d normalize();
    vector4d Q(vector4d);
    float dot(vector4d);
    float operator *(vector4d);
    vector4d cross(vector4d); // nope
    friend std::ostream& operator <<(std::ostream&, const vector4d);
    friend std::istream& operator >>(std::istream&, const vector4d);
};


#endif /* vector4d_h */
