#pragma once

#include "AbstractCamera.h"

class TargetCamera : public AbstractCamera
{
public:
	TargetCamera();
	~TargetCamera();

	void update() override;
	void rotate(const float yaw, const float pitch, const float roll) override;

	void setTarget(const glm::vec3 &tgt);
	glm::vec3 getTarget() const;

	void pan(const float dx, const float dy);
	void zoom(const float amount);
	void move(const float dx, const float dy);

protected:
	// Target position
	glm::vec3 target_;
	
	// Rotation limits
	float minRy_, maxRy_;

	// Distance between target and camera
	float distance_;

	// Distance limits
	float minDistance_, maxDistance_;

};
