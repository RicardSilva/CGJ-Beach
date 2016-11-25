#include "Camera.h"
#include "qtrn.h"

namespace engine {

	Camera::Camera() {}

	Camera::Camera(float near, float far) : _near(near), _far(far) {_eye = new vec3(); _center = new vec3(); _up = new vec3(); }

	Camera::~Camera() {
		delete _eye;
		delete _center;
		delete _up;
	}

	vec3 *Camera::getEye() { return _eye; }
	vec3 *Camera::getCenter() { return _center; }
	vec3 *Camera::getUp() { return _up; }

	void Camera::setEye(float x, float y, float z) { _eye->Set(x, y, z); }
	void Camera::setEye(const vec3 &v) { *_eye = v; }

	void Camera::setCenter(float x, float y, float z) { _center->Set(x, y, z); }
	void Camera::setCenter(const vec3 &v) { *_center = v; }

	void Camera::setUp(float x, float y, float z) { _up->Set(x, y, z); }
	void Camera::setUp(const vec3 &v) { *_up = v; }

	void Camera::setQtrn(qtrn q) { _q = q; }
	qtrn Camera::getQtrn() { return _q; }

	void Camera::setViewMatrix(mat4 &m) { viewMatrix = m; }
	mat4 Camera::getViewMatrix() { return viewMatrix; }

	void Camera::setProjMatrix(mat4 &m) { projMatrix = m; }
	mat4 Camera::getProjMatrix() { return projMatrix; }


	mat4 Camera::computeViewMatrix() {
		vec3 view = *_center - *_eye;
		view.Normalize();
		vec3 side = CrossProduct(view, *_up);
		side.Normalize();
		vec3 up = CrossProduct(side, view);
		up.Normalize();

		return mat4(side.x, side.y, side.z, -DotProduct(side, *_eye),
					up.x, up.y, up.z, -DotProduct(up, *_eye),
					-view.x, -view.y, -view.z, DotProduct(view, *_eye),
					0, 0, 0, 1);
	}
}