#pragma once

#include "vec.h"
#include "mat.h"
#include "qtrn.h"

using namespace std; 

namespace engine {

	class Camera {
	protected:
		vec3* _eye;
		vec3* _center;
		vec3* _up;
		float _near;
		float _far;

		vec4 clippingPlane;

		qtrn _q;

		mat4 viewMatrix;
		mat4 projMatrix;


	public:
		Camera();
		Camera(float near, float far);
		Camera(vec4 &v);

		virtual ~Camera();

		vec3 *getEye();
		vec3 *getCenter();
		vec3 *getUp();
		void setClippingPlane(vec4& v) { clippingPlane = v; }
		vec4 getClippingPlane() { return clippingPlane; }


		void setEye(float x, float y, float z);
		void setEye(const vec3 &v);

		void setCenter(float x, float y, float z);
		void setCenter(const vec3 &v);

		void setUp(float x, float y, float z);
		void setUp(const vec3 &v);

		void setQtrn(qtrn q);
		qtrn getQtrn();

		void setViewMatrix(mat4 &m);
		mat4 getViewMatrix();

		void setProjMatrix(mat4 &m);
		mat4 getProjMatrix();

		mat4 computeViewMatrix();
		virtual mat4 computeProjectionMatrix() { return mat4(); }
	};

}