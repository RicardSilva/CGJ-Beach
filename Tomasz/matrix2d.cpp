//
//  matrix2d.cpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 12/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//
#include "stdafx.h"
#include "matrix2d.h"
 
matrix2d::matrix2d(){
    for(int i=0;i<4;i++){
        M[i]=0;
    }
}

matrix2d::matrix2d(const float *mat){
    for(int i=0;i<4;i++){
        M[i]=mat[i];
    }
}

matrix2d::matrix2d(const float m11, const float m12,const float m21,const float m22){
    M[0]=m11;
    M[1]=m12;
    M[2]=m21;
    M[3]=m22;
}

float* matrix2d::getM(){
    return M;
}

void matrix2d::setM(const float *mat){
    for(int i=0;i<4;i++){
        M[i]=mat[i];
    }
}
matrix2d matrix2d::operator +(const matrix2d mat){
    matrix2d newMat=*new matrix2d(M);
    for(int i=0;i<4;i++){
        newMat.M[i]+=mat.M[i];
    }
    return newMat;
}

matrix2d matrix2d::operator -(const matrix2d mat){
    matrix2d newMat=*new matrix2d(M);
    for(int i=0;i<4;i++){
        newMat.M[i]-=mat.M[i];
    }
    return newMat;
}
matrix2d matrix2d::matrix2d::operator *(const float scalar){
    matrix2d newMat=*new matrix2d(M);
    for(int i=0;i<4;i++){
        newMat.M[i]*=scalar;
    }
    return newMat;
}

bool matrix2d::operator ==(const matrix2d mat){
    for(int i=0;i<4;i++){
        if(std::abs(M[i]-mat.M[i])>0.00001){
            std::cout<<std::abs(M[i]-mat.M[i])<<std::endl;
            return false;
        }
    }
    return true;
}

matrix2d matrix2d::T(){
    float mat[4];
    int n=2;
    for (int i=0;i<n*n;i++){
        int j = i/n;
        int k = i%n;
        mat[i] = M[n*k + j];
    }
    return *new matrix2d(mat);
}

float matrix2d::det(){
    return M[0]*M[3]-M[1]*M[2];
}

matrix2d matrix2d::inv(){
    if (det()!=0){
        return (*new matrix2d(M[3]/det(),-M[1]/det(),-M[2]/det(),M[0]/det()));
    }
    else
        return *this;
}

matrix2d matrix2d::operator *(const matrix2d mat){
    return *new matrix2d(M[0]*mat.M[0]+M[1]*mat.M[2],M[0]*mat.M[1]+M[1]*mat.M[3],
                         M[2]*mat.M[0]+M[3]*mat.M[2],M[2]*mat.M[1]+M[3]*mat.M[3]);
}

vector2d matrix2d::operator *(vector2d vec){
    return *new vector2d(M[0]*vec.getX()+M[1]*vec.getY(),M[2]*vec.getX()+M[3]*vec.getY());
}

void matrix2d::test(){
    matrix2d A = *new matrix2d(4,7,2,6);
    matrix2d B = *new matrix2d(2,3,4,7);
    matrix2d C = B.inv()*A.inv();
    matrix2d AB = (A*B).inv();
    std::cout<<A<<std::endl;
    std::cout<<A.T()<<std::endl;
    std::cout<<C<<std::endl;
    std::cout<<AB<<std::endl;
    if(C==AB)
        std::cout<<"equal"<<std::endl;
    else
        std::cout<<"not equal"<<std::endl;
}

std::ostream& operator <<(std::ostream& os, const matrix2d mat){
    return os<<"[ "<<mat.M[0]<<" "<<mat.M[1]<<" ]"<<std::endl<<"[ "<<mat.M[2]<<" "<<mat.M[3]<<" ]";
}
std::istream& operator >>(std::istream& is, matrix2d mat){
    for (int i=0;i<4;i++){
        is>>mat.M[i];
    }
    return is;
}
