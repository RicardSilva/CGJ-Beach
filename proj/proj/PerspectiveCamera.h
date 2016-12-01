#pragma once

#include "Camera.h"

namespace engine {

	class PerspectiveCamera : public Camera {

	private:
		float _fovy;
		float _aspect;

	public:
		PerspectiveCamera();
		PerspectiveCamera(float fovy, float aspect, float near, float far);
		~PerspectiveCamera();

		mat4 computeProjectionMatrix();

	};
}
