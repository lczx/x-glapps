#pragma once

#include "RenderableObject.h"

class TexturedPlane : public RenderableObject
{
public:
	TexturedPlane(const int width = 1000, const int depth = 1000);
	virtual ~TexturedPlane();

	int getTotalVertices() override;
	int getTotalIndices() override;
	GLenum getPrimitiveType() override;

	void fillVertexBuffer(GLfloat *pBuffer) override;
	void fillIndexBuffer(GLuint *pBuffer) override;

private:
	int width_, depth_;

};
