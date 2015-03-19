#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "../GLEngine.h"
#include "../../util/GLSLShader.h"

class SubdivisionEngine : public GLEngine
{
public:
	SubdivisionEngine();

private:
	void onRender() override;
	void onResize(int w, int h) override;
	void onShutdown() override;

	void onMouseDown(int button, int s, int x, int y) override;
	void onMouseMove(int x, int y) override;
	void onKey(unsigned char key, int x, int y) override;

	static const int TotalIndices = 3; // IT ALSO WORKS WITH ONLY A TRIANGLE

	// Shader 
	GLSLShader shader_;

	// Buffer object IDs
	GLuint vaoID_, vboVerticesID_, vboIndicesID_;

	// Mesh vertices and indices
	glm::vec3 vertices_[4];
	GLushort indices_[TotalIndices];

	// Projection matrix
	glm::mat4 P_ = glm::mat4(1);

	// Camera transformation variables
	int state_ = 0, oldX_ = 0, oldY_ = 0;
	float rX_ = .4f, rY_ = .0f, dist_ = -35;

	// Number of sub-divisions
	int subDivisions_ = 1;
	
};
