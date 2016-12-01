#pragma once

#include "Camera.h"

namespace engine {

	class OrthogonalCamera : public Camera {

	private:
		float _left;
		float _right;
		float _bottom;
		float _top;

	public:

		OrthogonalCamera();
		OrthogonalCamera(float left, float right, float bottom, float top, float near, float far);

		~OrthogonalCamera();

		mat4 computeProjectionMatrix();

	};
}