#pragma once

#include <glm/mat4x4.hpp>

#include "../objects/Plane.h"

class AbstractCamera
{
public:
	virtual ~AbstractCamera() { }

	// Camera setup and update on rotation
	
	void setupProjection(const float fovY, const float aspectRatio,
		const float near = 0.1f, const float far = 100.0f);

	virtual void update() = 0; // To recalculate matrices and orientation vectors
	virtual void rotate(const float yaw, const float pitch, const float roll);

	// View frustum-culling specific functions

	void calcFrustumPlanes();
	bool isPointInFrustum(const glm::vec3 &point);
	bool isSphereInFrustum(const glm::vec3 &center, const float radius);
	bool isBoxInFrustum(const glm::vec3 &min, const glm::vec3 &max);
	void getFrustumPlanes(glm::vec4 planes[6]);

	// Setters and getters (accessors and mutators)

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	glm::vec3 getPosition() const;
	void setPosition(const glm::vec3 &p);

	float getFOV() const;
	void setFOV(const float fov);
	float getAspectRatio() const;

	// Frustum points (TODO: make this private or protected?)
	glm::vec3 farPts[4];
	glm::vec3 nearPts[4];

protected:
	float yaw_, pitch_, roll_;
	float fov_, aspectRatio_;
	float zNear_ = .1f, zFar_ = 1000.f;

	static glm::vec3 UP;

	// The movement of the camera is based on these three orientation vectors
	glm::vec3 look_, up_, right_;

	glm::vec3 position_;

	glm::mat4 V_; // View matrix
	glm::mat4 P_; // Projection matrix

	Plane planes_[6]; // Frustum planes

};
