#include "AbstractCamera.h"

#include <glm/gtc/matrix_transform.hpp>

glm::vec3 AbstractCamera::UP = glm::vec3(0, 1, 0);

void AbstractCamera::setupProjection(const float fovY, const float aspectRatio, const float near, const float far)
{
	P_ = glm::perspective(fovY, aspectRatio, near, far);
	zNear_ = near;
	zFar_ = far;
	fov_ = fovY;
	aspectRatio_ = aspectRatio;
}

void AbstractCamera::rotate(const float yaw, const float pitch, const float roll)
{
	yaw_ = glm::radians(yaw);
	pitch_ = glm::radians(pitch);
	roll_ = glm::radians(roll);
	update();
}

void AbstractCamera::calcFrustumPlanes()
{
	// 'look_' should be normalized (i.e. norm, length = 1)

	// [cN, cF] is center of screen point with distance from camera [zNear, zFar]
	glm::vec3 cN = position_ + look_ * zNear_;
	glm::vec3 cF = position_ + look_ * zFar_;

	// TODO: Multiplication by 2.0f and division can be simplified

	// If zNear is ray and fov/2 is angle, the height of the triangle is hNear/2...
	float hNear = 2.0f * tan(glm::radians(fov_ / 2.0f)) * zNear_;
	float wNear = hNear * aspectRatio_;

	// Same
	float hFar = 2.0f * tan(glm::radians(fov_ / 2.0f)) * zFar_;
	float wFar = hFar * aspectRatio_;

	float hhNear = hNear / 2.0f;
	float hwNear = wNear / 2.0f;
	float hhFar = hFar / 2.0f;
	float hwFar = wFar / 2.0f;

	// Get angles of frustum
	farPts[0] = cF + up_ * hhFar - right_ * hwFar; // Far, top left
	farPts[1] = cF - up_ * hhFar - right_ * hwFar; // Far, bottom left
	farPts[2] = cF - up_ * hhFar + right_ * hwFar; // Far, bottom right
	farPts[3] = cF + up_ * hhFar + right_ * hwFar; // Far, top right

	nearPts[0] = cN + up_ * hhNear - right_ * hwNear; // Near, top left
	nearPts[1] = cN - up_ * hhNear - right_ * hwNear; // Near, bottom left
	nearPts[2] = cN - up_ * hhNear + right_ * hwNear; // Near, bottom right
	nearPts[3] = cN + up_ * hhNear + right_ * hwNear; // Near, top right

	planes_[0] = Plane::fromPoints(nearPts[3], nearPts[0], farPts[0]); // Top
	planes_[1] = Plane::fromPoints(nearPts[1], nearPts[2], farPts[2]); // Bottom
	planes_[2] = Plane::fromPoints(nearPts[0], nearPts[1], farPts[1]); // Left
	planes_[3] = Plane::fromPoints(nearPts[2], nearPts[3], farPts[2]); // Right
	planes_[4] = Plane::fromPoints(nearPts[0], nearPts[3], nearPts[2]); // Near
	planes_[5] = Plane::fromPoints(farPts[3], farPts[0], farPts[1]); // Far
	// Mmh... they seem counterclockwise from inside...
	// Is distance positive if inside and negative if outside?
}

bool AbstractCamera::isPointInFrustum(const glm::vec3 &point)
{
	for (int i = 0; i < 6; ++i)
		if (planes_[i].getDistance(point) < 0) return false;
	return true;
}

bool AbstractCamera::isSphereInFrustum(const glm::vec3 &center, const float radius)
{
	for (int i = 0; i < 6; ++i) {
		float d = planes_[i].getDistance(center);
		if (d < -radius) return false;
	}
	return true;
}

bool AbstractCamera::isBoxInFrustum(const glm::vec3 &min, const glm::vec3 &max)
{
	// Is 'n' used??? No.

	for (int i = 0; i < 6; ++i) {
		glm::vec3 p = min, n = max;
		glm::vec3 N = planes_[i].N;
		if (N.x >= 0) {
			p.x = max.x;
			n.x = min.x;
		}
		if (N.y >= 0) {
			p.y = max.y;
			n.y = min.y;
		}
		if (N.z >= 0) {
			p.z = max.z;
			n.z = min.z;
		}

		if (planes_[i].getDistance(p) < 0)
			return false;
	}
	return true;
}

void AbstractCamera::getFrustumPlanes(glm::vec4 planes[6])
{
	for (int i = 0; i < 6; ++i)
		planes[i] = glm::vec4(planes_[i].N, planes_[i].d);
}

glm::mat4 AbstractCamera::getViewMatrix() const
{
	return V_;
}

glm::mat4 AbstractCamera::getProjectionMatrix() const
{
	return P_;
}

glm::vec3 AbstractCamera::getPosition() const
{
	return position_;
}

void AbstractCamera::setPosition(const glm::vec3 &p)
{
	position_ = p;
}

float AbstractCamera::getFOV() const
{
	return fov_;
}

void AbstractCamera::setFOV(const float fov)
{
	// Fov is in degrees
	fov_ = fov;
	P_ = glm::perspective(fov_, aspectRatio_, zNear_, zFar_);
}

float AbstractCamera::getAspectRatio() const
{
	return aspectRatio_;
}
