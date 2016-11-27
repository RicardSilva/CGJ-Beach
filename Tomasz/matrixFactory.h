//
//  matrixFactory.hpp
//  ComputerGraphicsForGames_Lab1
//
//  Created by Tomasz Topczewski on 12/10/2016.
//  Copyright © 2016 Tomasz Topczewski. All rights reserved.
//

#ifndef matrixFactory_h
#define matrixFactory_h

#include <stdio.h>
#include "matrix2d.h"
#include "matrix3d.h"
#include "matrix4d.h"
namespace transformations {
	matrix4d T(float, float, float);
	matrix4d R(float, float, float, float);
	matrix4d S(float, float, float);
}
	class matrixFactory {
	public:
		static matrix2d createIndentityMat2();
		static matrix3d createIndentityMat3();
		static matrix4d createIndentityMat4();
		static matrix2d createZeroMat2();
		static matrix3d createZeroMat3();
		static matrix4d createZeroMat4();
		static matrix2d createRotationMat2(float);
		static matrix3d createRotationMat3(float, vector3d);
		static matrix3d createRotationMat3(float, float, float, float);
		static matrix4d createRotationMat4(float, vector3d);
		static matrix4d createRotationMat4(float, float, float, float);
		static matrix3d createTranslationMat3(vector2d);
		static matrix3d createTranslationMat3(float, float);
		static matrix4d createTranslationMat4(vector3d);
		static matrix4d createTranslationMat4(float, float, float);
		static matrix2d createScaleMat2(float, float);
		static matrix3d createScaleMat3(float, float, float);
		static matrix4d createScaleMat4(float, float, float);
		static matrix2d createOpenGLMat2(matrix2d);
		static matrix3d createOpenGLMat3(matrix3d);
		static matrix4d createOpenGLMat4(matrix4d);
		static matrix4d createViewMatrix(vector3d, vector3d, vector3d);
		static matrix4d createOrthographicMatrix(float, float, float, float, float, float);
		static matrix4d createProjectionMatrix(float, float, float, float);

		//    static matrix3d T(matrix3d);
		//    static matrix3d R(float,float);
		//    static matrix3d S(float;)
	};

#endif /* matrixFactory_hpp */
