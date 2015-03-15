#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "../GLEngine.h"
#include "../../util/Geometry.h"
#include "../../util/GLSLShader.h"

/** Out vertex structure for interleaved attributes */
struct VertexEx
{
	glm::vec3 position;
	glm::vec3 color;
};

class SimpleTriangleEngine : public GLEngine
{
public:
	SimpleTriangleEngine();

private:
	void onRender() override;
	void onResize(int w, int h) override;
	void onShutdown() override;

	// Geometry reference
	Geometry triangle_;

	// Shader reference
	GLSLShader shader_;

	// Projection and modelview matrices
	glm::mat4 P_ = glm::mat4(1);
	glm::mat4 MV_ = glm::mat4(1);

};
