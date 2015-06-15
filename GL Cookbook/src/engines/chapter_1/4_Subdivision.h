#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "../GLEngine.h"
#include "../../util/Geometry.h"
#include "../../util/GLSLShader.h"

class SubdivisionEngine : public GLEngine
{
public:
	SubdivisionEngine(bool instanced);

private:
	void onRender() override;
	void onResize(int w, int h) override;
	void onShutdown() override;

	void onMouseDown(int button, int s, int x, int y) override;
	void onMouseMove(int x, int y) override;
	void onKey(unsigned char key, int x, int y) override;

	static const int TotalVertices = 3; // IT ALSO WORKS WITH ONLY A TRIANGLE (was 4)
	static const int TotalIndices = 3; // IT ALSO WORKS WITH ONLY A TRIANGLE (was 6)

	// Geometry and shader 
	Geometry geom_;
	GLSLShader shader_;

	// Projection matrix
	glm::mat4 P_ = glm::mat4(1);

	// Camera transformation variables
	int state_ = 0, oldX_ = 0, oldY_ = 0;
	float rX_ = .4f, rY_ = .0f, dist_ = -35;

	// Number of sub-divisions
	int subDivisions_ = 1;

	// 'true' if in instanced mode
	bool instanced_;

	// Instance modeling matrices (used only if operating in instanced mode)
	glm::mat4 M_[4];
	
};
