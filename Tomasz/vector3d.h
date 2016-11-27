//
//  vector3d.hpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 07/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//

#ifndef vector3d_h
#define vector3d_h

#include "vector2d.h"

class vector3d{
    float x;
    float y;
    float z;
public:
    vector3d(float,float,float);
    vector3d(vector2d,float);
    float getX();
    float getY();
    float getZ();
    void setX(float);
    void setY(float);
    void setZ(float);
    vector3d operator +(vector3d);
    vector3d operator -(vector3d);
    vector3d operator *(float);
    bool operator ==(vector3d);
    float norm();
    vector3d normalize();
    vector3d Q(vector3d);
    float dot(vector3d);
    float operator *(vector3d);
    vector3d cross(vector3d);
    friend std::ostream& operator <<(std::ostream&, const vector3d);
    friend std::istream& operator >>(std::istream&, const vector3d);
    void test();
};

#endif /* vector3d_hpp */
