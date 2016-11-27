//
//  matrix4d.cpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 12/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//
#include "stdafx.h"
#include "matrix4d.h"
 
const float mThreshold = (float)1.0e-5;

matrix4d::matrix4d(){
    for(int i=0;i<16;i++){
        M[i]=0;
    }
}

matrix4d::matrix4d(const float *mat){
    for(int i=0;i<16;i++){
        M[i]=mat[i];
    }
}

matrix4d::matrix4d(const float m11, const float m12,const float m13,const float m14,
                   const float m21, const float m22,const float m23,const float m24,
                   const float m31, const float m32,const float m33,const float m34,
                   const float m41, const float m42,const float m43,const float m44){
    M[0]=m11;
    M[1]=m12;
    M[2]=m13;
    M[3]=m14;
    M[4]=m21;
    M[5]=m22;
    M[6]=m23;
    M[7]=m24;
    M[8]=m31;
    M[9]=m32;
    M[10]=m33;
    M[11]=m34;
    M[12]=m41;
    M[13]=m42;
    M[14]=m43;
    M[15]=m44;
    
}

matrix4d::matrix4d(vector4d column1, vector4d column2, vector4d column3, vector4d column4) {
	M[0] = column1.getX();
	M[1] = column2.getX();
	M[2] = column3.getX();
	M[3] = column4.getX();
	M[4] = column1.getY();
	M[5] = column2.getY();
	M[6] = column3.getY();
	M[7] = column4.getY();
	M[8] = column1.getZ();
	M[9] = column2.getZ();
	M[10] = column3.getZ();
	M[11] = column4.getZ();
	M[12] = column1.getW();
	M[13] = column2.getW();
	M[14] = column3.getW();
	M[15] = column4.getW();
}

matrix4d::matrix4d(matrix3d mat){
    M[0]=mat.getM()[0];
    M[1]=mat.getM()[1];
    M[2]=mat.getM()[2];
    M[3]=0;
    M[4]=mat.getM()[3];
    M[5]=mat.getM()[4];
    M[6]=mat.getM()[5];
    M[7]=0;
    M[8]=mat.getM()[6];
    M[9]=mat.getM()[7];
    M[10]=mat.getM()[8];
    M[11]=0;
    M[12]=0;
    M[13]=0;
    M[14]=0;
    M[15]=1;
}

const void matrix4d::mClean()
{
	for (int i = 0; i < 16; i++) {
		if (fabs(M[i]) < mThreshold) M[i] = 0.0f;
	}
}

float* matrix4d::getM(){
    return M;
}

void matrix4d::setM(const float *mat){
    for(int i=0;i<16;i++){
        M[i]=mat[i];
    }
}
matrix4d matrix4d::operator +(const matrix4d mat){
    matrix4d newMat=*new matrix4d(M);
    for(int i=0;i<16;i++){
        newMat.M[i]+=mat.M[i];
    }
    return newMat;
}

matrix4d matrix4d::operator -(const matrix4d mat){
    matrix4d newMat=*new matrix4d(M);
    for(int i=0;i<16;i++){
        newMat.M[i]-=mat.M[i];
    }
    return newMat;
}
matrix4d matrix4d::matrix4d::operator *(const float scalar){
    matrix4d newMat=*new matrix4d(M);
    for(int i=0;i<16;i++){
        newMat.M[i]*=scalar;
    }
    return newMat;
}

bool matrix4d::operator ==(const matrix4d mat){
    for(int i=0;i<16;i++){
        if(std::abs(M[i]-mat.M[i])>FLT_EPSILON*10)
            return false;
    }
    return true;
}

matrix4d matrix4d::T(){
    float mat[16];
    int n=4;
    for (int i=0;i<n*n;i++){
        int j = i/n;
        int k = i%n;
        mat[i] = M[n*k + j];
    }
    return *new matrix4d(mat);
}


float matrix4d::det(){
   return M[3] * M[6] * M[9] * M[12] - M[2] * M[7] * M[9] * M[12] -
    M[3] * M[5] * M[10] * M[12] + M[1] * M[7] * M[10] * M[12] +
    M[2] * M[5] * M[11] * M[12] - M[1] * M[6] * M[11] * M[12] -
    M[3] * M[6] * M[8] * M[13] + M[2] * M[7] * M[8] * M[13] +
    M[3] * M[4] * M[10] * M[13] - M[0] * M[7] * M[10] * M[13] -
    M[2] * M[4] * M[11] * M[13] + M[0] * M[6] * M[11] * M[13] +
    M[3] * M[5] * M[8] * M[14] - M[1] * M[7] * M[8] * M[14] -
    M[3] * M[4] * M[9] * M[14] + M[0] * M[7] * M[9] * M[14] +
    M[1] * M[4] * M[11] * M[14] - M[0] * M[5] * M[11] * M[14] -
    M[2] * M[5] * M[8] * M[15] + M[1] * M[6] * M[8] * M[15] +
    M[2] * M[4] * M[9] * M[15] - M[0] * M[6] * M[9] * M[15] -
    M[1] * M[4] * M[10] * M[15] + M[0] * M[5] * M[10] * M[15];
}

matrix4d matrix4d::inv(){
    if (det()!=0){
        float *inv=new float[16];
        inv[0] = M[5]  * M[10] * M[15] -
        M[5]  * M[11] * M[14] -
        M[9]  * M[6]  * M[15] +
        M[9]  * M[7]  * M[14] +
        M[13] * M[6]  * M[11] -
        M[13] * M[7]  * M[10];
        
        inv[4] = -M[4]  * M[10] * M[15] +
        M[4]  * M[11] * M[14] +
        M[8]  * M[6]  * M[15] -
        M[8]  * M[7]  * M[14] -
        M[12] * M[6]  * M[11] +
        M[12] * M[7]  * M[10];
        
        inv[8] = M[4]  * M[9] * M[15] -
        M[4]  * M[11] * M[13] -
        M[8]  * M[5] * M[15] +
        M[8]  * M[7] * M[13] +
        M[12] * M[5] * M[11] -
        M[12] * M[7] * M[9];
        
        inv[12] = -M[4]  * M[9] * M[14] +
        M[4]  * M[10] * M[13] +
        M[8]  * M[5] * M[14] -
        M[8]  * M[6] * M[13] -
        M[12] * M[5] * M[10] +
        M[12] * M[6] * M[9];
        
        inv[1] = -M[1]  * M[10] * M[15] +
        M[1]  * M[11] * M[14] +
        M[9]  * M[2] * M[15] -
        M[9]  * M[3] * M[14] -
        M[13] * M[2] * M[11] +
        M[13] * M[3] * M[10];
        
        inv[5] = M[0]  * M[10] * M[15] -
        M[0]  * M[11] * M[14] -
        M[8]  * M[2] * M[15] +
        M[8]  * M[3] * M[14] +
        M[12] * M[2] * M[11] -
        M[12] * M[3] * M[10];
        
        inv[9] = -M[0]  * M[9] * M[15] +
        M[0]  * M[11] * M[13] +
        M[8]  * M[1] * M[15] -
        M[8]  * M[3] * M[13] -
        M[12] * M[1] * M[11] +
        M[12] * M[3] * M[9];
        
        inv[13] = M[0]  * M[9] * M[14] -
        M[0]  * M[10] * M[13] -
        M[8]  * M[1] * M[14] +
        M[8]  * M[2] * M[13] +
        M[12] * M[1] * M[10] -
        M[12] * M[2] * M[9];
        
        inv[2] = M[1]  * M[6] * M[15] -
        M[1]  * M[7] * M[14] -
        M[5]  * M[2] * M[15] +
        M[5]  * M[3] * M[14] +
        M[13] * M[2] * M[7] -
        M[13] * M[3] * M[6];
        
        inv[6] = -M[0]  * M[6] * M[15] +
        M[0]  * M[7] * M[14] +
        M[4]  * M[2] * M[15] -
        M[4]  * M[3] * M[14] -
        M[12] * M[2] * M[7] +
        M[12] * M[3] * M[6];
        
        inv[10] = M[0]  * M[5] * M[15] -
        M[0]  * M[7] * M[13] -
        M[4]  * M[1] * M[15] +
        M[4]  * M[3] * M[13] +
        M[12] * M[1] * M[7] -
        M[12] * M[3] * M[5];
        
        inv[14] = -M[0]  * M[5] * M[14] +
        M[0]  * M[6] * M[13] +
        M[4]  * M[1] * M[14] -
        M[4]  * M[2] * M[13] -
        M[12] * M[1] * M[6] +
        M[12] * M[2] * M[5];
        
        inv[3] = -M[1] * M[6] * M[11] +
        M[1] * M[7] * M[10] +
        M[5] * M[2] * M[11] -
        M[5] * M[3] * M[10] -
        M[9] * M[2] * M[7] +
        M[9] * M[3] * M[6];
        
        inv[7] = M[0] * M[6] * M[11] -
        M[0] * M[7] * M[10] -
        M[4] * M[2] * M[11] +
        M[4] * M[3] * M[10] +
        M[8] * M[2] * M[7] -
        M[8] * M[3] * M[6];
        
        inv[11] = -M[0] * M[5] * M[11] +
        M[0] * M[7] * M[9] +
        M[4] * M[1] * M[11] -
        M[4] * M[3] * M[9] -
        M[8] * M[1] * M[7] +
        M[8] * M[3] * M[5];
        
        inv[15] = M[0] * M[5] * M[10] -
        M[0] * M[6] * M[9] -
        M[4] * M[1] * M[10] +
        M[4] * M[2] * M[9] +
        M[8] * M[1] * M[6] -
        M[8] * M[2] * M[5];inv[0] = M[5]  * M[10] * M[15] -
        M[5]  * M[11] * M[14] -
        M[9]  * M[6]  * M[15] +
        M[9]  * M[7]  * M[14] +
        M[13] * M[6]  * M[11] -
        M[13] * M[7]  * M[10];
        
        inv[4] = -M[4]  * M[10] * M[15] +
        M[4]  * M[11] * M[14] +
        M[8]  * M[6]  * M[15] -
        M[8]  * M[7]  * M[14] -
        M[12] * M[6]  * M[11] +
        M[12] * M[7]  * M[10];
        
        inv[8] = M[4]  * M[9] * M[15] -
        M[4]  * M[11] * M[13] -
        M[8]  * M[5] * M[15] +
        M[8]  * M[7] * M[13] +
        M[12] * M[5] * M[11] -
        M[12] * M[7] * M[9];
        
        inv[12] = -M[4]  * M[9] * M[14] +
        M[4]  * M[10] * M[13] +
        M[8]  * M[5] * M[14] -
        M[8]  * M[6] * M[13] -
        M[12] * M[5] * M[10] +
        M[12] * M[6] * M[9];
        
        inv[1] = -M[1]  * M[10] * M[15] +
        M[1]  * M[11] * M[14] +
        M[9]  * M[2] * M[15] -
        M[9]  * M[3] * M[14] -
        M[13] * M[2] * M[11] +
        M[13] * M[3] * M[10];
        
        inv[5] = M[0]  * M[10] * M[15] -
        M[0]  * M[11] * M[14] -
        M[8]  * M[2] * M[15] +
        M[8]  * M[3] * M[14] +
        M[12] * M[2] * M[11] -
        M[12] * M[3] * M[10];
        
        inv[9] = -M[0]  * M[9] * M[15] +
        M[0]  * M[11] * M[13] +
        M[8]  * M[1] * M[15] -
        M[8]  * M[3] * M[13] -
        M[12] * M[1] * M[11] +
        M[12] * M[3] * M[9];
        
        inv[13] = M[0]  * M[9] * M[14] -
        M[0]  * M[10] * M[13] -
        M[8]  * M[1] * M[14] +
        M[8]  * M[2] * M[13] +
        M[12] * M[1] * M[10] -
        M[12] * M[2] * M[9];
        
        inv[2] = M[1]  * M[6] * M[15] -
        M[1]  * M[7] * M[14] -
        M[5]  * M[2] * M[15] +
        M[5]  * M[3] * M[14] +
        M[13] * M[2] * M[7] -
        M[13] * M[3] * M[6];
        
        inv[6] = -M[0]  * M[6] * M[15] +
        M[0]  * M[7] * M[14] +
        M[4]  * M[2] * M[15] -
        M[4]  * M[3] * M[14] -
        M[12] * M[2] * M[7] +
        M[12] * M[3] * M[6];
        
        inv[10] = M[0]  * M[5] * M[15] -
        M[0]  * M[7] * M[13] -
        M[4]  * M[1] * M[15] +
        M[4]  * M[3] * M[13] +
        M[12] * M[1] * M[7] -
        M[12] * M[3] * M[5];
        
        inv[14] = -M[0]  * M[5] * M[14] +
        M[0]  * M[6] * M[13] +
        M[4]  * M[1] * M[14] -
        M[4]  * M[2] * M[13] -
        M[12] * M[1] * M[6] +
        M[12] * M[2] * M[5];
        
        inv[3] = -M[1] * M[6] * M[11] +
        M[1] * M[7] * M[10] +
        M[5] * M[2] * M[11] -
        M[5] * M[3] * M[10] -
        M[9] * M[2] * M[7] +
        M[9] * M[3] * M[6];
        
        inv[7] = M[0] * M[6] * M[11] -
        M[0] * M[7] * M[10] -
        M[4] * M[2] * M[11] +
        M[4] * M[3] * M[10] +
        M[8] * M[2] * M[7] -
        M[8] * M[3] * M[6];
        
        inv[11] = -M[0] * M[5] * M[11] +
        M[0] * M[7] * M[9] +
        M[4] * M[1] * M[11] -
        M[4] * M[3] * M[9] -
        M[8] * M[1] * M[7] +
        M[8] * M[3] * M[5];
        
        inv[15] = M[0] * M[5] * M[10] -
        M[0] * M[6] * M[9] -
        M[4] * M[1] * M[10] +
        M[4] * M[2] * M[9] +
        M[8] * M[1] * M[6] -
        M[8] * M[2] * M[5];
        
		mClean();

        for (int i = 0; i < 16; i++)
            inv[i] = inv[i]/det();
        
        return *new matrix4d(inv);
    }
    else
        return *this;
}

matrix4d matrix4d::operator *(const matrix4d mat){
    
    float mat_r[16];
    
    for (unsigned int i = 0; i < 16; i += 4)
        for (unsigned int j = 0; j < 4; ++j)
            mat_r[i + j] = (M[i + 0] * mat.M[j +  0])
            + (M[i + 1] * mat.M[j +  4])
            + (M[i + 2] * mat.M[j +  8])
            + (M[i + 3] * mat.M[j + 12]);

    return mat_r;
}

vector4d matrix4d::operator *(vector4d vec){
    return *new vector4d(M[0]*vec.getX()+M[1]*vec.getY()+M[2]*vec.getZ()+M[3]*vec.getW(),
                         M[4]*vec.getX()+M[5]*vec.getY()+M[6]*vec.getZ()+M[7]*vec.getW(),
                         M[8]*vec.getX()+M[9]*vec.getY()+M[10]*vec.getZ()+M[11]*vec.getW(),
                         M[12]*vec.getX()+M[13]*vec.getY()+M[14]*vec.getZ()+M[15]*vec.getW());
}



std::ostream& operator <<(std::ostream& os, const matrix4d mat){
    os<<"[ "<<mat.M[0]<<" "<<mat.M[1]<<" "<<mat.M[2]<<" "<<mat.M[3]<<" ]"<<std::endl;
    os<<"[ "<<mat.M[4]<<" "<<mat.M[5]<<" "<<mat.M[6]<<" "<<mat.M[7]<<" ]"<<std::endl;
    os<<"[ "<<mat.M[8]<<" "<<mat.M[9]<<" "<<mat.M[10]<<" "<<mat.M[11]<<" ]"<<std::endl;
    os<<"[ "<<mat.M[12]<<" "<<mat.M[13]<<" "<<mat.M[14]<<" "<<mat.M[15]<<" ]";
    return os;
    
    
}
std::istream& operator >>(std::istream& is, matrix4d mat){
    for (int i=0;i<16;i++){
        is>>mat.M[i];
    }
    return is;
}


void matrix4d::test(){
    //B^(-1)A^(-1)=C^(-1)=(AB)^(-1).
    matrix4d A = *new matrix4d(2,5,3,5,4,6,6,3,11,3,2,-2,4,-7,9,3);
    matrix4d B = *new matrix4d(3,2,1521,0,32,13,20,0,23,7,1,0,0,0,0,1);
    matrix4d C = B.inv()*A.inv();
    matrix4d AB = B*A;
    std::cout<<A<<std::endl;
    std::cout<<A.T()<<std::endl;
    std::cout<<std::endl;
    std::cout<<B<<std::endl;
    std::cout<<C<<std::endl;
    std::cout<<AB<<std::endl;
    if(C==AB)
        std::cout<<"equal"<<std::endl;
    matrix4d D = *new matrix4d(2,5,3,5,4,6,6,3,11,3,2,-2,4,-7,9,3);
    vector4d V = *new vector4d(1,1,1,1);
    std::cout<<D*V<<std::endl;

}

