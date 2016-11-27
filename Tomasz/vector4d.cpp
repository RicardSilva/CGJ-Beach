//
//  vector4d.cpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 12/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//
#include "stdafx.h"
#include "vector4d.h"

vector4d::vector4d() {
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

vector4d::vector4d(vector3d vec){
    x=vec.getX();
    y=vec.getY();
    z=vec.getZ();
    w=1;
}

vector4d::vector4d(vector3d vec, float _w){
    x=vec.getX();
    y=vec.getY();
    z=vec.getZ();
    w=_w;
}

vector4d::vector4d(GLfloat * vec) {
	//if (vec.length == 4) {
		x = vec[0];
		y = vec[1];
		z = vec[2];
		w = vec[3];
	//}
}

vector4d::vector4d(float _x,float _y, float _z, float _w){
    x=_x;
    y=_y;
    z=_z;
    w=_w;
}

float vector4d::getX(){
    return x;
}

float vector4d::getY(){
    return y;
}

float vector4d::getZ(){
    return z;
}

float vector4d::getW(){
    return w;
}

void vector4d::setX(float _x){
    x=_x;
}

void vector4d::setY(float _y){
    y=_y;
}

void vector4d::setZ(float _z){
    z=_z;
}

void vector4d::setW(float _w){
    w=_w;
}


vector4d vector4d::operator +(vector4d vec){
    return *new vector4d(x+vec.x,y+vec.y,z+vec.z,w+vec.w);
}

vector4d vector4d::operator -(vector4d vec){
    return *new vector4d(x-vec.x,y-vec.y,z-vec.z,w-vec.w);
}

vector4d vector4d::operator *(float scalar){
    return *new vector4d(x*scalar,y*scalar,z*scalar,w*scalar);
}

bool vector4d::operator ==(vector4d vec){
    return (x==vec.x&&y==vec.y&&z==vec.z&w==vec.w);
}

float vector4d::norm(){
    return sqrt(x*x+y*y+z*z+w*w);
}

vector4d vector4d::normalize()
{
	vector4d vn;
	float s = 1 / (w * sqrt(x*x + y*y + z*z));
	vn.x = x * s;
	vn.y = y * s;
	vn.z = z * s;
	vn.w = 1.0f;
	return vn;
}


vector4d vector4d::Q(vector4d vec){
    return *new vector4d(x*x-vec.x*vec.x,y*y-vec.y*vec.y,z*z-vec.z*vec.z,w*w-vec.w*vec.w);
}

float vector4d::dot(vector4d vec){
    return x*vec.x+y*vec.y+z*vec.z+w*vec.w;
}

float vector4d::operator *(vector4d vec){
    return dot(vec);
}

vector4d vector4d::cross(vector4d vec){
//    return *new vector4d(y*vec.z-z*vec.y,z*vec.x-x*vec.z,x*vec.y-y*vec.x,);
    return *new vector4d(0,0,0,0); //not implemented
}

std::ostream& operator<<(std::ostream& os, const vector4d vec){
    return os<<"( "<<vec.x<<", "<<vec.y<<", "<<vec.z<<", "<<vec.w<<" )";
}

std::istream& operator>>(std::istream& is, vector4d vec){
    return is>>vec.x>>vec.y>>vec.z>>vec.w;
}
