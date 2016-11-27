//
//  matrixFactory.cpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 12/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//

#define _USE_MATH_DEFINES 
#include <cmath>

#include "stdafx.h"
#include "matrixFactory.h"

matrix2d matrixFactory::createIndentityMat2(){
    return *new matrix2d(1,0,0,1);
}

 matrix3d matrixFactory::createIndentityMat3(){
    return *new matrix3d(1,0,0,0,1,0,0,0,1);
}
 matrix4d matrixFactory::createIndentityMat4(){
    return *new matrix4d(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
}

 matrix2d matrixFactory::createZeroMat2(){
    return *new matrix2d(0,0,0,0);
}
 matrix3d matrixFactory::createZeroMat3(){
    return *new matrix3d(0,0,0,0,0,0,0,0,0);
}
 matrix4d matrixFactory::createZeroMat4(){
    return *new matrix4d(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
}

 matrix2d matrixFactory::createRotationMat2(float theta){
    return *new matrix2d(cosf(theta),-sinf(theta),sinf(theta),cosf(theta));
}

 matrix3d matrixFactory::createRotationMat3(float theta,float ax,float ay,float az){
	theta = theta*3.14 / 180; // degrees
    matrix3d A=*new matrix3d(0 ,  -az,  ay,
                             az,  0 , -ax,
                             -ay, ax,   0 );
    matrix3d A2=*new matrix3d(-az*az-ay*ay,ax*ay,ax*az,
                              ax*ay,-az*az-ax*ax,ay*az,
                              ax*az,ay*az,-ay*ay-ax*ax);
    matrix3d R=createIndentityMat3()+A*sinf(theta)+A2*(1-cosf(theta));
    return R;
}

matrix3d matrixFactory::createRotationMat3(float theta,vector3d a){
    float ax=a.getX();
    float ay=a.getY();
    float az=a.getZ();
    return createRotationMat3(theta, ax, ay, az);
}

matrix4d matrixFactory::createRotationMat4(float theta,float ax,float ay, float az){
    return *new matrix4d(createRotationMat3(theta,ax,ay,az));
}

matrix4d matrixFactory::createRotationMat4(float theta,vector3d a){
    return *new matrix4d(createRotationMat3(theta,a));
}

 matrix3d matrixFactory::createTranslationMat3(float x,float y){
    return *new matrix3d(1,0,x,
                         0,1,y,
                         0,0,1);
}

matrix3d matrixFactory::createTranslationMat3(vector2d vec){
    float x=vec.getX();
    float y=vec.getY();
    return createTranslationMat3(x,y);
}

matrix4d matrixFactory::createTranslationMat4(float x,float y,float z){
    return *new matrix4d(1,0,0,x,
                         0,1,0,y,
                         0,0,1,z,
                         0,0,0,1);
}

matrix4d matrixFactory::createTranslationMat4(vector3d vec){
    float x=vec.getX();
    float y=vec.getY();
    float z=vec.getZ();
    return createTranslationMat4(x,y,z);
}

 matrix2d matrixFactory::createScaleMat2(float sx,float sy){
    return *new matrix2d(sx,0,
                         0,sy);
}

 matrix3d matrixFactory::createScaleMat3(float sx,float sy,float sz){
    return *new matrix3d(sx,0,0,
                         0,sy,0,
                         0,0,sz);
}

matrix4d matrixFactory::createScaleMat4(float sx,float sy,float sz){
    return *new matrix4d(createScaleMat3(sx, sy, sz));
}

matrix2d matrixFactory::createOpenGLMat2(matrix2d mat){
    return mat.T();
}

matrix3d matrixFactory::createOpenGLMat3(matrix3d mat){
    return mat.T();
}

matrix4d matrixFactory::createOpenGLMat4(matrix4d mat){
    return mat.T();
}

namespace transformations {
	matrix4d T(float x, float y, float z) {
		return matrixFactory::createTranslationMat4(x, y, z);
	}
	matrix4d R(float alfa, float x, float y, float z) {
		return matrixFactory::createRotationMat4(alfa, x, y, z);
	}
	matrix4d S(float x, float y, float z) {
		return matrixFactory::createScaleMat4(x, y, z);
	}
}

matrix4d matrixFactory::createViewMatrix(vector3d eye, vector3d target, vector3d up){
	vector3d zaxis = (eye - target).normalize();
	vector3d xaxis = (up.cross(zaxis)).normalize();
	vector3d yaxis = zaxis.cross(xaxis);
	return (*new matrix4d(vector4d(xaxis.getX(), yaxis.getX(), zaxis.getX(), 0),
		vector4d(xaxis.getY(), yaxis.getY(), zaxis.getY(), 0),
		vector4d(xaxis.getZ(), yaxis.getZ(), zaxis.getZ(), 0),
		vector4d(-(xaxis*eye), -(yaxis*eye), -(zaxis*eye), 1))).T();
}

matrix4d matrixFactory::createOrthographicMatrix(float l, float r, float t, float b, float n, float f) {
	return (*new matrix4d(2/(r-l), 0          , 0           , (l+r)/(l-r),
					     0      , 2 / (t - b), 0           , (t+b)/(b-t),
		                 0      , 0          , -2 / (f - n) , (n+f)/(n-f),
		                 0      , 0          , 0           , 1)).T();
}

 matrix4d matrixFactory::createProjectionMatrix(float fov, float aspect, float n, float f) {
	 float D2R = 3.14 / 180.0;
	 float yScale = 1.0 / tan(D2R * fov / 2);
	 float xScale = yScale / aspect;
	 float nearmfar = n - f;
	return (*new matrix4d(xScale, 0, 0, 0,
		0, yScale, 0, 0,
		0, 0, (f + n) / nearmfar, -1,
		0, 0, 2 * f*n / nearmfar, 0));
}