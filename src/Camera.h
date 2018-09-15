#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "VkBootstrapHeader.h"
#include "Viewport.h"

namespace VkBootstrap
{
	class Camera
	{
	protected:
		OpenML::Mat4f projectionMatrix;
		OpenML::Mat4f viewMatrix;

		OpenML::Vec3f position;
		OpenML::Vec3f target;
		OpenML::Vec3f up;

		// Untransformed corners of the frustum
		OpenML::Vec4f nearUL, nearLL, nearUR, nearLR;
		OpenML::Vec4f farUL, farLL, farUR, farLR;

		// Transformed corners of Frustum
		OpenML::Vec4f nearULT, nearLLT, nearURT, nearLRT;
		OpenML::Vec4f farULT, farLLT, farURT, farLRT;

		// Base and Transformed plane equations
		OpenML::Vec4f nearPlane, farPlane, leftPlane, rightPlane;
		OpenML::Vec4f topPlane, bottomPlane;

	public:
		void init(const OpenML::Vec3f& position, const OpenML::Vec3f& target, const OpenML::Vec3f& up = OpenML::Vec3f(0.0f, 1.0f, 0.0f));

		OpenML::Vec3f& getPosition();
		void setPosition(const OpenML::Vec3f& newPostion);

		OpenML::Vec3f& getTarget();
		void setTarget(const OpenML::Vec3f& newTarget);

		OpenML::Mat4f& getProjectionMatrix();
		OpenML::Mat4f& getViewMatrix();

		void setPerspective(float fieldOfView, float aspect, float nearFrustum, float farFrustum);
		void setOrthographic(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

		OpenML::Mat4f getHUDProjectionMatrix(float width, float height);

		OpenML::Point3Df getFromWorldToScreen(const OpenML::Point3Df& vertex, const OpenML::Mat4f& modelMatrix, Viewport* viewport);

		void updateViewMatrix();
		void update(long long elapsedTime);
	};
}

#endif