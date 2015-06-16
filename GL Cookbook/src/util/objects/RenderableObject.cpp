#include "RenderableObject.h"

#include <glm/vec3.hpp>

RenderableObject::RenderableObject() { }

RenderableObject::~RenderableObject()
{
	destroy();
}

void RenderableObject::init()
{
	// Setup VAO and VBO stuff
	glGenVertexArrays(1, &vaoID_);
	glGenBuffers(1, &vboVerticesID_);
	glGenBuffers(1, &vboIndicesID_);

	// Get total vertices and indices
	totalVertices_ = getTotalVertices();
	totalIndices_ = getTotalIndices();
	primType_ = getPrimitiveType();

	// Now allocate buffers
	glBindVertexArray(vaoID_);

	glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID_);
	glBufferData(GL_ARRAY_BUFFER, totalVertices_ * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);

	GLfloat *pBuffer = static_cast<GLfloat *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	fillVertexBuffer(pBuffer); // Call to implementation
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glEnableVertexAttribArray(shader_["vVertex"]);
	glVertexAttribPointer(shader_["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalIndices_ * sizeof(GLuint), nullptr, GL_STATIC_DRAW);

	GLuint *pIBuffer = static_cast<GLuint *>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	fillIndexBuffer(pIBuffer); // Call to implementation
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	glBindVertexArray(0);
}

void RenderableObject::destroy()
{
	// Destroy shader
	shader_.deleteShaderProgram();

	// Destroy VAO and VBO
	glDeleteBuffers(1, &vboVerticesID_);
	glDeleteBuffers(1, &vboIndicesID_);
	glDeleteVertexArrays(1, &vaoID_);
}

void RenderableObject::render(const float *MVP)
{
	shader_.use();
	glUniformMatrix4fv(shader_("MVP"), 1, GL_FALSE, MVP);
	setCustomUniforms(); // Call to implementation
	glBindVertexArray(vaoID_);
	glDrawElements(primType_, totalIndices_, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	shader_.unuse();
}
