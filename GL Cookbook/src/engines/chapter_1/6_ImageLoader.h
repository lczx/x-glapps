#pragma once

#include <glm/vec2.hpp>

#include "../GLEngine.h"
#include "../../util/Geometry.h"
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

	// Geometry and shader reference
	Geometry geom_;
	GLSLShader shader_;

	// Texture ID for fullscreen quad
	GLuint textureID_;
	
};
