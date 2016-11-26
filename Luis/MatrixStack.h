#ifndef MatrixStack_H
#define MatrixStack_H

#include <vector>

#include "Matrix4x4.h"
#include "MatrixFactory.h"

#define PI 3.14159265

class MatrixStack {
public:
	std::vector<Matrix4x4> stack;
	MatrixFactory mf;

	MatrixStack() {}

	MatrixStack(MatrixFactory m) {
		mf = m;
		stack.push_back(mf.identity4());
	}

	void pushMatrix() {
		stack.push_back(stack.back());
	}

	void popMatrix() {
		if (stack.size() > 1)
			stack.pop_back();
		else
			stack[0] = mf.identity4();
	}

	void translate(Vector3 vec) {
		stack[stack.size() - 1] = stack[stack.size() - 1] * mf.createTranslation4(vec);
	}

	void scale(Vector3 vec) {
		stack[stack.size() - 1] = stack[stack.size() - 1] * mf.createScale4(vec);
	}

	void rotate(float angle, Vector3 vec) {
		stack[stack.size() - 1] = stack[stack.size() - 1] * mf.createRotation(vec, angle);
	}

	Matrix4x4 getModelMatrix() {
		return stack.back();
	}

	void complexTransform(Matrix4x4 mat){
		stack[stack.size() - 1] = stack[stack.size() - 1] * mat;
	}
};


#endif