#pragma once

#include "AbstractCamera.h"

class FreeCamera : public AbstractCamera
{
public:
	void update() override;

	void walk(const float dt);
	void strafe(const float dt);
	void lift(const float dt);

	void setTranslation(const glm::vec3 &t);
	glm::vec3 getTranslation() const;

	void setSpeed(const float speed);
	const float getSpeed() const;

protected:
	// Stores the current translation (default constructs at glm::vec3(0))
	glm::vec3 translation_;

	// Move speed of camera in m/s
	float speed_ = .5f;

};
