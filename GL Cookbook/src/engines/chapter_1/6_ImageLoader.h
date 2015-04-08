#pragma once

#include <glm/vec2.hpp>

#include "../GLEngine.h"
#include "../../util/GLSLShader.h"

class ImageLoaderEngine : public GLEngine
{
public:
	explicit ImageLoaderEngine(const char *texturePath);

private:
	void onRender() override;
	void onResize(int w, int h) override;
	void onShutdown() override;

	static const int TotalVertices = 4;
	static const int TotalIndices = 6;

	// Texture loading utility
	void loadTexture();

	// Texture image filename
	std::string texturePath_;

	// Shader reference
	GLSLShader shader_;

	// Quad vertices and indices
	glm::vec2 vertices_[TotalVertices];
	GLushort indices_[TotalIndices];

	// Vertex array, vertex buffer object and texture ID for fullscreen quad
	GLuint vaoID_, vboVerticesID_, vboIndicesID_, textureID_;
	
};
