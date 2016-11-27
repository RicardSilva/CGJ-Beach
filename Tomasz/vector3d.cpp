//
//  vector3d.cpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 07/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//

#include "stdafx.h"
#include "vector3d.h"


vector3d::vector3d(float _x,float _y, float _z){
    x=_x;
    y=_y;
    z=_z;
}

vector3d::vector3d(vector2d vec, float _z){
    x=vec.getX();
    y=vec.getY();
    z=_z;
}

float vector3d::getX(){
    return x;
}

float vector3d::getY(){
    return y;
}

float vector3d::getZ(){
    return z;
}

void vector3d::setX(float _x){
    x=_x;
}

void vector3d::setY(float _y){
    y=_y;
}

void vector3d::setZ(float _z){
    z=_z;
}

vector3d vector3d::operator +(vector3d vec){
    return *new vector3d(x+vec.x,y+vec.y,z+vec.z);
}

vector3d vector3d::operator -(vector3d vec){
    return *new vector3d(x-vec.x,y-vec.y,z-vec.z);
}

vector3d vector3d::operator *(float scalar){
    return *new vector3d(x*scalar,y*scalar,z*scalar);
}

bool vector3d::operator ==(vector3d vec){
    return ((std::abs(x-vec.x)<FLT_EPSILON)&&(std::abs(y-vec.y)<FLT_EPSILON)&&(std::abs(z-vec.z)<FLT_EPSILON));
}

float vector3d::norm(){
    return sqrtf(x*x+y*y+z*z);
}

vector3d vector3d::normalize(){
    if(norm()==0)
        return *new vector3d(0,0,0);
    else
        return *new vector3d(x/norm(),y/norm(),z/norm());
}

vector3d vector3d::Q(vector3d vec){
    return *new vector3d(x*x-vec.x*vec.x,y*y-vec.y*vec.y,z*z-vec.z*vec.z);
}

float vector3d::dot(vector3d vec){
    return x*vec.x+y*vec.y+z*vec.z;
}

float vector3d::operator *(vector3d vec){
    return dot(vec);
}

void vector3d::test(){
    srand (time_t(NULL));
    for (int l=0;l<10;l++){
        vector3d i=*new vector3d((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
        vector3d j=*new vector3d((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
        vector3d k=*new vector3d((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
        vector3d vec1=i.cross(j.cross(k));
        vector3d vec2=j*(i*k)-k*(i*j);
        std::cout<<i<<std::endl;
        std::cout<<j<<std::endl;
        std::cout<<k<<std::endl;
        std::cout<<vec1<<std::endl;
        std::cout<<vec2<<std::endl;
        if(vec1==vec2)
            std::cout<<"equal"<<std::endl;
        std::cout<<std::endl;
    }
}

vector3d vector3d::cross(vector3d vec){
    return *new vector3d(y*vec.z-z*vec.y,z*vec.x-x*vec.z,x*vec.y-y*vec.x);
}

std::ostream& operator<<(std::ostream& os, const vector3d vec){
    return os<<"( "<<vec.x<<", "<<vec.y<<", "<<vec.z<<" )";
}

std::istream& operator>>(std::istream& is, vector3d vec){
    return is>>vec.x>>vec.y>>vec.z;
}
