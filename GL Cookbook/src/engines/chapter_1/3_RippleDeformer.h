#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "../GLEngine.h"
#include "../../util/GLSLShader.h"

// Out vertex format
typedef glm::vec3 Vertex;

class RippleDeformerEngine : public GLEngine
{
public:
	RippleDeformerEngine() : GLEngine(GLE_REGISTER_ALL ^ GLE_REG_KEY) { }
	void onInit() override;

private:
	void onRender() override;
	void onResize(int w, int h) override;
	void onShutdown() override;

	void onMouseDown(int button, int s, int x, int y) override;
	void onMouseMove(int x, int y) override;

	void onIdle() override;

	// Total quads on X axis and Z axis
	static const int NumX = 40, NumZ = 40;

	// Vertices count
	// ... 3 verts for each triangle * 2 triangles for a quad
	static const int TotalIndices = NumX * NumZ * 2 * 3;

	// Size of plane in world space
	static const float SizeX, SizeZ;

	// Ripple displacement speed
	static const float Speed;

	// Shader
	GLSLShader shader_;

	// Buffer object IDs
	GLuint vaoID_, vboVerticesID_, vboIndicesID_;

	// Ripple mesh vertices and indices
	Vertex vertices_[(NumX + 1) * (NumZ + 1)];
	GLushort indices_[TotalIndices];

	// Projection matrix
	glm::mat4 P_;

	// Camera transformation variables
	int state_ = 0, oldX_ = 0, oldY_ = 0;
	float rX_ = .3f, rY_ = .4f, dist_ = -7;
	
	// Current time
	float time_ = 0;

};
