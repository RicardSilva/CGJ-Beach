//
//  vector2d.hpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 07/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//

#ifndef vector2d_h
#define vector2d_h

#include <stdio.h>
#include <iostream>

class vector2d{
    float x;
    float y;
public:
    vector2d(float,float);
    float const getX();
    float const getY();
    void setX(float);
    void setY(float);
    vector2d operator +(vector2d);
    vector2d operator -(vector2d);
    vector2d operator *(float);
    bool operator ==(vector2d);
    float norm();
    vector2d normalize();
    vector2d Q(vector2d);
    friend std::ostream& operator <<(std::ostream&, const vector2d);
    friend std::istream& operator >>(std::istream&, const vector2d);
};


#endif /* vector2d_hpp */
