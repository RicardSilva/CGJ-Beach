//
//  vector2d.cpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 07/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//
#include "stdafx.h"
#include "vector2d.h"

vector2d::vector2d(float _x,float _y){
        x=_x;
        y=_y;
}

float const vector2d::getX(){
        return x;
}
    
float const vector2d::getY(){
        return y;
}
    
void vector2d::setX(float _x){
        x=_x;
}

void vector2d::setY(float _y){
        y=_y;
}

vector2d vector2d::operator +(vector2d vec){
    return *new vector2d(x+vec.x,y+vec.y);
}
                        
vector2d vector2d::operator -(vector2d vec){
    return *new vector2d(x-vec.x,y-vec.y);
}

vector2d vector2d::operator *(float scalar){
    return *new vector2d(x*scalar,y*scalar);
}

bool vector2d::operator ==(vector2d vec){
    return ((std::abs(x-vec.x)<FLT_EPSILON)&&(std::abs(y-vec.y)<FLT_EPSILON));
}

float vector2d::norm(){
    return sqrtf(x*x+y*y);
}

vector2d vector2d::normalize(){
    if(norm()==0)
        return *new vector2d(0,0);
    else
        return *new vector2d(x/norm(),y/norm());
}

vector2d vector2d::Q(vector2d vec){
    return *new vector2d(x*x-vec.x*vec.x,y*y-vec.y*vec.y);
}

std::ostream& operator<<(std::ostream& os, const vector2d vec){
    return os<<"( "<<vec.x<<", "<<vec.y<<" )";
}

std::istream& operator>>(std::istream& is, vector2d vec){
    return is>>vec.x>>vec.y;
}
