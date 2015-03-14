#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "../../util/GLSLShader.h"
#include "../GLEngine.h"

/** Out vertex structure for interleaved attributes */
struct VertexEx
{
	glm::vec3 position;
	glm::vec3 color;
};

class SimpleTriangleEngine : public GLEngine
{
public:
	SimpleTriangleEngine() : GLEngine(GLE_REGISTER_DISPLAY) { }
	void onInit() override;

private:
	void onRender() override;
	void onResize(int w, int h) override;
	void onShutdown() override;

	// Shader reference
	GLSLShader shader_;

	// Vertex array and vertex buffer object IDs
	GLuint vaoID_, vboVerticesID_, vboIndicesID_;

	// Triangle vertices and indices
	VertexEx vertices_[3];
	GLushort indices_[3];

	// Projection and modelview matrices
	glm::mat4 P_ = glm::mat4(1);
	glm::mat4 MV_ = glm::mat4(1);

};
