#include "TargetCamera.h"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

TargetCamera::TargetCamera() : minRy_(-60), maxRy_(60), minDistance_(1), maxDistance_(10)
{
	// Parent class fields do not exist yet in the initializer list, they
	// are only created on constructor call, so we initialize them here.
	right_ = glm::vec3(1, 0, 0);
	up_ = glm::vec3(0, 1, 0);
	look_ = glm::vec3(0, 0, -1);

	// 'target_' [vec3] is default-initialized
	// 'distance_' [float] is primitive and IS NOT initialized
}

TargetCamera::~TargetCamera() { }

void TargetCamera::update()
{
	// Calculate the new orientation (rotation) matrix using the current camera orientations.
	glm::mat4 R = glm::yawPitchRoll(yaw_, pitch_, roll_); // or 0.0f roll?

	// Use the distance to get a vector and then translate it by the current rotation matrix.
	glm::vec3 T = glm::vec3(0, 0, distance_);
	T = glm::vec3(R * glm::vec4(T, 0.0f));

	// Get the new camera position by adding the translation vector to the target position.
	position_ = target_ + T;

	// Recalculate the orthonormal basis and then the view matrix.
	look_ = glm::normalize(target_ - position_);
	up_ = glm::vec3(R * glm::vec4(UP, 0.0f));
	right_ = glm::cross(look_, up_);
	V_ = glm::lookAt(position_, target_, up_);
}

void TargetCamera::rotate(const float yaw, const float pitch, const float roll)
{
	float p = std::min(std::max(pitch, minRy_), maxRy_);
	AbstractCamera::rotate(yaw, p, roll);
}

void TargetCamera::setTarget(const glm::vec3 &tgt)
{
	target_ = tgt;
	distance_ = glm::distance(position_, target_);
	distance_ = std::max(minDistance_, std::min(distance_, maxDistance_));

	/*V_ = glm::lookAt(position, target, up);
	m_yaw = 0;
	m_pitch = 0;
	if (V[0][0] < 0) m_yaw = glm::degrees((float)(M_PI)-asinf(-V[2][0])));
	else m_yaw = glm::degrees(asinf(-V[2][0]));
	m_pitch = glm::degrees(asinf(-V[1][2]));
	*/
}

glm::vec3 TargetCamera::getTarget() const
{
	return target_;
}

void TargetCamera::pan(const float dx, const float dy)
{
	// Pan moves in the XY plane only,
	// hence the up vector is used instead of the look vector.
	glm::vec3 X = right_ * dx;
	glm::vec3 Y = up_ * dy;
	position_ += X + Y;
	target_ += X + Y;
	update();
}

void TargetCamera::zoom(const float amount)
{
	// This moves the position in the look direction
	position_ += look_ * amount;
	distance_ = glm::distance(position_, target_);
	distance_ = std::max(minDistance_, std::min(distance_, maxDistance_));
	update();
}

void TargetCamera::move(const float dx, const float dy)
{
	// This moves both the position and target by the same amount
	// in both look and right vector directions.
	glm::vec3 X = right_ * dx;
	glm::vec3 Y = look_ * dy;
	position_ += X + Y;
	target_ += X + Y;
	update();
}
