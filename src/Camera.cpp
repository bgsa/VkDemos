#include "Camera.h"

namespace VkBootstrap
{
	void Camera::init(const OpenML::Vec3f& position, const OpenML::Vec3f& target, const OpenML::Vec3f& up)
	{
		this->position[0] = position[0];
		this->position[1] = position[1];
		this->position[2] = position[2];

		this->target[0] = target[0];
		this->target[1] = target[1];
		this->target[2] = target[2];

		this->up[0] = up[0];
		this->up[1] = up[1];
		this->up[2] = up[2];
	}

	void Camera::setPosition(const Vec3f& newPostion)
	{
		this->position[0] = newPostion[0];
		this->position[1] = newPostion[1];
		this->position[2] = newPostion[2];
	}

	void Camera::setTarget(const Vec3f& newTarget)
	{
		this->target[0] = newTarget[0];
		this->target[1] = newTarget[1];
		this->target[2] = newTarget[2];
	}

	Vec3f& Camera::getPosition()
	{
		return position;
	}

	Vec3f& Camera::getTarget()
	{
		return target;
	}

	Mat4f& Camera::getProjectionMatrix()
	{
		return projectionMatrix;
	}

	Mat4f& Camera::getViewMatrix()
	{
		return viewMatrix;
	}

	void Camera::setPerspective(float fieldOfView, float aspect, float nearFrustum, float farFrustum)
	{
		float xmin, xmax, ymin, ymax;       // Dimensions of near clipping plane
		float xFmin, xFmax, yFmin, yFmax;   // Dimensions of far clipping plane

		// Do the Math for the near clipping plane
		ymax = nearFrustum * tanf(float(fieldOfView * PI_DIV_360));
		ymin = -ymax;
		xmin = ymin * aspect;
		xmax = -xmin;

		// Construct the projection matrix
		projectionMatrix = Mat4f::identity();
		projectionMatrix[0] = (2.0f * nearFrustum) / (xmax - xmin);
		projectionMatrix[5] = (2.0f * nearFrustum) / (ymax - ymin);
		projectionMatrix[8] = (xmax + xmin) / (xmax - xmin);
		projectionMatrix[9] = (ymax + ymin) / (ymax - ymin);
		projectionMatrix[10] = -((farFrustum + nearFrustum) / (farFrustum - nearFrustum));
		projectionMatrix[11] = -1.0f;
		projectionMatrix[14] = -((2.0f * farFrustum * nearFrustum) / (farFrustum - nearFrustum));
		projectionMatrix[15] = 0.0f;

		// Do the Math for the far clipping plane
		yFmax = farFrustum * tanf(float(fieldOfView * PI_DIV_360));
		yFmin = -yFmax;
		xFmin = yFmin * aspect;
		xFmax = -xFmin;

		// Fill in values for untransformed Frustum corners
		// Near Upper Left
		nearUL[0] = xmin;
		nearUL[1] = ymax;
		nearUL[2] = -nearFrustum;
		nearUL[3] = 1.0f;

		// Near Lower Left
		nearLL[0] = xmin;
		nearLL[1] = ymin;
		nearLL[2] = -nearFrustum;
		nearLL[3] = 1.0f;

		// Near Upper Right
		nearUR[0] = xmax;
		nearUR[1] = ymax;
		nearUR[2] = -nearFrustum;
		nearUR[3] = 1.0f;

		// Near Lower Right
		nearLR[0] = xmax;
		nearLR[1] = ymin;
		nearLR[2] = -nearFrustum;
		nearLR[3] = 1.0f;

		// Far Upper Left
		farUL[0] = xFmin;
		farUL[1] = yFmax;
		farUL[2] = -farFrustum;
		farUL[3] = 1.0f;

		// Far Lower Left
		farLL[0] = xFmin;
		farLL[1] = yFmin;
		farLL[2] = -farFrustum;
		farLL[3] = 1.0f;

		// Far Upper Right
		farUR[0] = xFmax;
		farUR[1] = yFmax;
		farUR[2] = -farFrustum;
		farUR[3] = 1.0f;

		// Far Lower Right
		farLR[0] = xFmax;
		farLR[1] = yFmin;
		farLR[2] = -farFrustum;
		farLR[3] = 1.0f;

		updateViewMatrix();
	}

	void Camera::setOrthographic(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
	{
		projectionMatrix = Mat4f::createOrthographicMatrix(xMin, xMax, yMin, yMax, zMin, zMax);

		// Fill in values for untransformed Frustum corners	// Near Upper Left
		nearUL[0] = xMin;
		nearUL[1] = yMax;
		nearUL[2] = zMin;
		nearUL[3] = 1.0f;

		// Near Lower Left
		nearLL[0] = xMin;
		nearLL[1] = yMin;
		nearLL[2] = zMin;
		nearLL[3] = 1.0f;

		// Near Upper Right
		nearUR[0] = xMax;
		nearUR[1] = yMax;
		nearUR[2] = zMin;
		nearUR[3] = 1.0f;

		// Near Lower Right
		nearLR[0] = xMax;
		nearLR[1] = yMin;
		nearLR[2] = zMin;
		nearLR[3] = 1.0f;

		// Far Upper Left
		farUL[0] = xMin;
		farUL[1] = yMax;
		farUL[2] = zMax;
		farUL[3] = 1.0f;

		// Far Lower Left
		farLL[0] = xMin;
		farLL[1] = yMin;
		farLL[2] = zMax;
		farLL[3] = 1.0f;

		// Far Upper Right
		farUR[0] = xMax;
		farUR[1] = yMax;
		farUR[2] = zMax;
		farUR[3] = 1.0f;

		// Far Lower Right
		farLR[0] = xMax;
		farLR[1] = yMin;
		farLR[2] = zMax;
		farLR[3] = 1.0f;

		updateViewMatrix();
	}

	Mat4f Camera::getHUDProjectionMatrix(float width, float height)
	{
		Mat4f matrix = Mat4f::createOrthographicMatrix(0.0f, width, 0.0f, height, -1.0f, 1.0f);

		return matrix;
	}

	void Camera::updateViewMatrix()
	{
		Vec3f cameraDirection = (position - target).normalize(); //zAxis
		Vec3f cameraRight = up.cross(cameraDirection).normalize(); //xAxis
		Vec3f cameraUp = cameraDirection.cross(cameraRight); //yAxis

		Mat4f cameraMatrix = {
			cameraRight[0], cameraUp[0], cameraDirection[0], 0.0f,
			cameraRight[1], cameraUp[1], cameraDirection[1], 0.0f,
			cameraRight[2], cameraUp[2], cameraDirection[2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		Mat4f translation = Mat4f::createTranslate(-position[0], -position[1], -position[2]);

		viewMatrix = cameraMatrix * translation;
	}

	void Camera::update(long long elapsedTime)
	{
		updateViewMatrix();
	}

	OpenML::Point3Df Camera::getFromWorldToScreen(const OpenML::Point3Df& vertex, const OpenML::Mat4f& modelMatrix, Viewport* viewport)
	{
		Size viewportSize = viewport->getSize();

		float halhWidth = viewportSize.width / 2.0f;
		float halhHeight = viewportSize.height / 2.0f;

		//Vec4f vertex4D = Vec4f(vertex, 1.0f) * modelViewMatrix * viewMatrix * projectionMatrix;
		Vec4f vertex4D = projectionMatrix * viewMatrix * modelMatrix * Vec4f(vertex.toVec3(), 1.0f);
		vertex4D /= vertex4D[3];

		Vec3f vertexOnDeviceSpace = vertex4D.toVec3();

		OpenML::Point3Df vertexViewportSpace = {
			(vertexOnDeviceSpace[0] + 1.0f) * halhWidth,
			(vertexOnDeviceSpace[1] + 1.0f) * halhHeight,
			vertex4D[3]
		};

		return vertexViewportSpace;
	}
}