#include "TexturedPlane.h"

#include <glm/vec3.hpp>

#include "../glassert.h"

char SHADER_TEXTURED_FRAME_VERT[] = R"(
#version 330 core
layout(location = 0) in vec3 vVertex; // object space position
uniform mat4 MVP; // combined modelview projection
smooth out vec2 vUV; // 2D texture coordinate

void main() {
	// Multiply the combined MVP matrix with the
	// object space position to get the clip space position
	gl_Position = MVP * vec4(vVertex.xyz, 1);

	// Get the input vertex x and z value as the 2D texture coordinate
	vUV = (vVertex.xz);
}
)";

const char SHADER_TEXTURED_FRAME_FRAG[] = R"(
#version 330 core
layout(location = 0) out vec4 vFragColor; // fragment output color
smooth in vec2 vUV; // interpolated 2D texture coords
uniform sampler2D textureMap; // texture map

void main() {
	// Use the interpolated texture coordinate too lookup
	// the color from the given texture map
	//vFragColor = texture(textureMap, vUV).rrrr;
	vFragColor = texture(textureMap, vUV);
}
)";

TexturedPlane::TexturedPlane(const int width, const int depth) : width_(width), depth_(depth)
{
	// Setup shader
	shader_.loadFromString(GL_VERTEX_SHADER, SHADER_TEXTURED_FRAME_VERT);
	shader_.loadFromString(GL_FRAGMENT_SHADER, SHADER_TEXTURED_FRAME_FRAG);
	shader_.createAndLinkProgram();

	GL_CHECK_ERRORS;

	shader_.use();
	shader_.addAttribute("vVertex");
	shader_.addUniform("MVP");
	shader_.addUniform("textureMap");
	glUniform1i(shader_("textureMap"), 0);
	shader_.unuse();

	GL_CHECK_ERRORS;

	init();

	GL_CHECK_ERRORS;
}

TexturedPlane::~TexturedPlane() { }

int TexturedPlane::getTotalVertices() { return 4; }

int TexturedPlane::getTotalIndices() { return 6; }

GLenum TexturedPlane::getPrimitiveType() { return GL_TRIANGLES; }

void TexturedPlane::fillVertexBuffer(GLfloat *pBuffer)
{
	glm::vec3 *vertices = reinterpret_cast<glm::vec3 *>(pBuffer);

	int width_2 = width_ / 2;
	int depth_2 = depth_ / 2;

	vertices[0] = glm::vec3(-width_2, 0, -depth_2);
	vertices[1] = glm::vec3(width_2, 0, -depth_2);
	vertices[2] = glm::vec3(width_2, 0, depth_2);
	vertices[3] = glm::vec3(-width_2, 0, depth_2);
}

void TexturedPlane::fillIndexBuffer(GLuint *pBuffer)
{
	// Fill indices array
	GLuint *id = pBuffer;
	id[0] = 0; id[1] = 1; id[2] = 2;
	id[3] = 0; id[4] = 2; id[5] = 3;
}
