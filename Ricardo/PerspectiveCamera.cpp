#include "PerspectiveCamera.h"
#include <math.h>
namespace engine {

	PerspectiveCamera::PerspectiveCamera() {}
	PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far) : Camera(near, far), _fovy(fovy), _aspect(aspect) {}

	PerspectiveCamera::~PerspectiveCamera() {}


	mat4 PerspectiveCamera::computeProjectionMatrix() {
		
		// CONVERT FOV FROM DEGREES TO RADIANS
		float d = 1 / tan(((_fovy * PI) / 180) / 2);

		return mat4(d/_aspect,      0,                         0,                           0,
						    0,      d,                         0,                           0,
						    0,      0, (_far+_near)/(_near-_far), (2*_far*_near)/(_near-_far),
			                0,      0,                        -1,                           0);

	}

	


}