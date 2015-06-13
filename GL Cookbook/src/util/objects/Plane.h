#pragma once

#include <glm/vec3.hpp>

class Plane
{
	enum Where { COPLANAR, FRONT, BACK };

public:
	Plane();
	Plane(const glm::vec3 &N, const glm::vec3 &p);
	~Plane();

	static Plane fromPoints(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);
	Where classify(const glm::vec3 &p);
	float getDistance(const glm::vec3 &p);

	glm::vec3 N;
	float d;

};
