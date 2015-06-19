#include "FreeCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

void FreeCamera::update()
{
	// Calculate the new orientation (rotation) matrix using the current camera orientations.
	glm::mat4 R = glm::yawPitchRoll(yaw_, pitch_, roll_);

	// Translate the camera position by the current translation amount.
	position_ += translation_;

	// set this when no movement decay is needed (defined in engine implementation)
	// translation = glm::vec3(0);

	// Transform the look vector by the current rotation matrix,
	// and determine the right and up vectors to calculate the orthonormal basis.
	look_ = glm::vec3(R * glm::vec4(0, 0, 1, 0));
	up_ = glm::vec3(R * glm::vec4(0, 1, 0, 0));
	right_ = glm::cross(look_, up_);

	// Determine the camera target point, and use "lookAt" to calculate
	// the new view matrix using camera position, target and up vector.
	glm::vec3 tgt = position_ + look_;
	V_ = glm::lookAt(position_, tgt, up_);
}

void FreeCamera::walk(const float dt)
{
	// Simply translate the camera in the "look" direction
	translation_ += look_ * speed_ * dt;
	update();
}

void FreeCamera::strafe(const float dt)
{
	// Translate the camera in the "right" direction
	translation_ += right_ * speed_ * dt;
	update();
}

void FreeCamera::lift(const float dt)
{
	// Translate the camera in the "up" direction
	translation_ += up_ * speed_ * dt;
	update();
}

void FreeCamera::setTranslation(const glm::vec3 &t)
{
	translation_ = t;
	update();
}

glm::vec3 FreeCamera::getTranslation() const
{
	return translation_;
}

void FreeCamera::setSpeed(const float speed)
{
	speed_ = speed;
}

const float FreeCamera::getSpeed() const
{
	return speed_;
}
