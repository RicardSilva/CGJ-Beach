//
//  matrix3d.cpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 12/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//
#include "stdafx.h"
#include "matrix3d.h"
 
matrix3d::matrix3d(){
    for(int i=0;i<9;i++){
        M[i]=0;
    }
}

matrix3d::matrix3d(const float *mat){
    for(int i=0;i<9;i++){
        M[i]=mat[i];
    }
}

matrix3d::matrix3d(const float m11, const float m12,const float m13,
                   const float m21, const float m22,const float m23,
                   const float m31, const float m32,const float m33){
    M[0]=m11;
    M[1]=m12;
    M[2]=m13;
    M[3]=m21;
    M[4]=m22;
    M[5]=m23;
    M[6]=m31;
    M[7]=m32;
    M[8]=m33;
}

matrix3d::matrix3d(vector3d row1, vector3d row2, vector3d row3) {
	M[0] = row1.getX();
	M[1] = row1.getY();
	M[2] = row1.getZ();
	M[3] = row2.getX();
	M[4] = row2.getY();
	M[5] = row2.getZ();
	M[6] = row3.getX();
	M[7] = row3.getY();
	M[7] = row3.getZ();
}


matrix3d::matrix3d(matrix2d mat){
    float m11=mat.getM()[0];
    float m12=mat.getM()[1];
    float m21=mat.getM()[2];
    float m22=mat.getM()[3];
    new matrix3d(m11,m12,0,m21,m22,0,0,0,1);
}

float* matrix3d::getM(){
    return M;
}

void matrix3d::setM(const float *mat){
    for(int i=0;i<9;i++){
        M[i]=mat[i];
    }
}
matrix3d matrix3d::operator +(const matrix3d mat){
    matrix3d newMat=*new matrix3d(M);
    for(int i=0;i<9;i++){
        newMat.M[i]+=mat.M[i];
    }
    return newMat;
}

matrix3d matrix3d::operator -(const matrix3d mat){
    matrix3d newMat=*new matrix3d(M);
    for(int i=0;i<9;i++){
        newMat.M[i]-=mat.M[i];
    }
    return newMat;
}
matrix3d matrix3d::matrix3d::operator *(const float scalar){
    matrix3d newMat=*new matrix3d(M);
    for(int i=0;i<9;i++){
        newMat.M[i]*=scalar;
    }
    return newMat;
}

bool matrix3d::operator ==(const matrix3d mat){
    for(int i=0;i<9;i++){
        if(std::abs(M[i]-mat.M[i])>FLT_EPSILON)
            return false;
    }
    return true;
}

matrix3d matrix3d::T(){
    float mat[9];
    int n=3;
    for (int i=0;i<n*n;i++){
        int j = i/n;
        int k = i%n;
        mat[i] = M[n*k + j];
    }
    return *new matrix3d(mat);
}

float matrix3d::det(){
    return M[0]*(M[4]*M[8]-M[5]*M[7])-M[1]*(M[3]*M[8]-M[5]*M[6])+M[2]*(M[3]*M[7]-M[4]*M[6]);
}

matrix3d matrix3d::inv(){
    if (det()!=0){
        return (*new matrix3d((M[4]*M[8]-M[5]*M[7])/det(),(M[2]*M[7]-M[1]*M[8])/det(),(M[1]*M[5]-M[2]*M[4])/det(),
                              (M[5]*M[6]-M[3]*M[8])/det(),(M[0]*M[8]-M[2]*M[6])/det(),(M[2]*M[3]-M[0]*M[5])/det(),
                              (M[3]*M[7]-M[4]*M[6])/det(),(M[1]*M[6]-M[0]*M[7])/det(),(M[0]*M[4]-M[1]*M[3])/det()));
    }
    else
        return *this;
}

matrix3d matrix3d::operator *(const matrix3d mat){
    return *new matrix3d(M[0]*mat.M[0]+M[1]*mat.M[3]+M[2]*mat.M[6],M[0]*mat.M[1]+M[1]*mat.M[4]+M[2]*mat.M[7],M[0]*mat.M[2]+M[1]*mat.M[5]+M[2]*mat.M[8],
                         M[3]*mat.M[0]+M[4]*mat.M[3]+M[5]*mat.M[6],M[3]*mat.M[1]+M[4]*mat.M[4]+M[5]*mat.M[7],M[3]*mat.M[2]+M[4]*mat.M[5]+M[5]*mat.M[8],
                         M[6]*mat.M[0]+M[7]*mat.M[3]+M[8]*mat.M[6],M[6]*mat.M[1]+M[7]*mat.M[4]+M[8]*mat.M[7],M[6]*mat.M[2]+M[7]*mat.M[5]+M[8]*mat.M[8]);
}

vector3d matrix3d::operator *(vector3d vec){
    return *new vector3d(M[0]*vec.getX()+M[1]*vec.getY()+M[2]*vec.getZ(),M[3]*vec.getX()+M[4]*vec.getY()+M[5]*vec.getZ(),M[6]*vec.getX()+M[7]*vec.getY()+M[8]*vec.getZ());
}

std::ostream& operator <<(std::ostream& os, const matrix3d mat){
    os<<"[ "<<mat.M[0]<<" "<<mat.M[1]<<" "<<mat.M[2]<<" ]"<<std::endl;
    os<<"[ "<<mat.M[3]<<" "<<mat.M[4]<<" "<<mat.M[5]<<" ]"<<std::endl;
    os<<"[ "<<mat.M[6]<<" "<<mat.M[7]<<" "<<mat.M[8]<<" ]";
    return os;


}
std::istream& operator >>(std::istream& is, matrix3d mat){
    for (int i=0;i<9;i++){
        is>>mat.M[i];
    }
    return is;
}


void matrix3d::test(){
    //B^(-1)A^(-1)=C^(-1)=(AB)^(-1).
    matrix3d A = *new matrix3d(5,-2,1,0,3,-1,2,0,7);
    matrix3d B = *new matrix3d(3,2,1521,32,13,20,23,7,1);
    matrix3d C = B.inv()*A.inv();
    matrix3d AB = (A*B).inv();
    std::cout<<B<<std::endl;
    std::cout<<B.T()<<std::endl;
    std::cout<<AB<<std::endl;
    if(C==AB)
        std::cout<<"equal"<<std::endl;
}

