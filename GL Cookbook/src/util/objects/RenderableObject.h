#pragma once

#include "../GLSLShader.h"

// TODO: Maybe we can use "../Geometry.h" here.

class RenderableObject
{
public:
	RenderableObject();
	virtual ~RenderableObject();

	virtual int getTotalVertices() = 0;
	virtual int getTotalIndices() = 0;
	virtual GLenum getPrimitiveType() = 0;

	virtual void fillVertexBuffer(GLfloat *pBuffer) = 0;
	virtual void fillIndexBuffer(GLuint *pBuffer) = 0;

	virtual void setCustomUniforms() { }

	void init();
	void destroy();

	void render(const float *MVP);

protected:
	GLuint vaoID_, vboVerticesID_, vboIndicesID_;

	GLSLShader shader_;

	GLenum primType_;
	int totalVertices_, totalIndices_;

};
