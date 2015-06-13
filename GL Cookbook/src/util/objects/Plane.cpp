#include "Plane.h"

#include <glm/geometric.hpp>

const float Epsilon = .0001f;

Plane::Plane()
	: N(glm::vec3(0, 1, 0)), d(0) { }

Plane::Plane(const glm::vec3 &normal, const glm::vec3 &p)
	: N(normal), d(-glm::dot(N, p)) { }

Plane::~Plane() { }

Plane Plane::fromPoints(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3)
{
	Plane plane;
	glm::vec3 e1 = v2 - v1;
	glm::vec3 e2 = v3 - v1;
	plane.N = glm::normalize(glm::cross(e1, e2));
	plane.d = -glm::dot(plane.N, v1);
	return plane;
}

Plane::Where Plane::classify(const glm::vec3 &p)
{
	auto res = getDistance(p);
	if (res > Epsilon) return FRONT;
	if (res < Epsilon) return BACK;
	return COPLANAR;
}

float Plane::getDistance(const glm::vec3 &p)
{
	return glm::dot(N, p) + d;
}
