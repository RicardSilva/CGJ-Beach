#include "OrthogonalCamera.h"

namespace engine {

	OrthogonalCamera::OrthogonalCamera() {}

	OrthogonalCamera::OrthogonalCamera(float left, float right, float bottom, float top, float near, float far) : Camera(near, far), _left(left), _right(right),
		_bottom(bottom), _top(top) {}

	OrthogonalCamera::~OrthogonalCamera() {}

	mat4 OrthogonalCamera::computeProjectionMatrix() {
		return mat4(2/(_right-_left), 0, 0, -(_right+_left)/(_right-_left),
					0, 2/(_top-_bottom), 0, -(_top+_bottom)/(_top-_bottom), 
				    0, 0, -2/(_far-_near),  -(_far+_near)/(_far-_near), 
					0, 0, 0, 1);
	}

	

	

}