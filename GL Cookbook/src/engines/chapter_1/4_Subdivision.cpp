#include "4_Subdivision.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SubdivisionEngine::SubdivisionEngine() : GLEngine(GLE_REGISTER_ALL ^ GLE_REG_IDLE)
{
	GL_CHECK_ERRORS;

	// Temporary snippet to show max. geometry shader output.
	// Since my laptop's graphics card isn't fully compliant to OpenGL 3.3, so this
	// doesn't work, at least for me. However the whole test runs fine.
	//   GLint maxVerts;
	//   glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &maxVerts);
	//   if (glGetError() == GL_NO_ERROR)
	//     cout << "Max. geometry shader output : " << maxVerts << '\n';
	//   else // GL_INVALID_ENUM
	//     cout << "Cannot retrieve GL_MAX_GEOMETRY_OUTPUT_VERTICES (min. GL version: 3.2).\n";

	// Load, create and link the shader
	shader_.loadFromFile(GL_VERTEX_SHADER, "shaders/a4_subdiv_vert.glsl");
	shader_.loadFromFile(GL_GEOMETRY_SHADER, "shaders/a4_subdiv_geom.glsl");
	shader_.loadFromFile(GL_FRAGMENT_SHADER, "shaders/a4_subdiv_frag.glsl");
	shader_.createAndLinkProgram();

	// Add attributes and uniforms
	shader_.use();
	shader_.addAttribute("vVertex");
	shader_.addUniform("MVP");
	shader_.addUniform("sub_divisions");
	//! try to enable this line and change uniform value on key event.
	//glUniform1i(shader_("sub_divisions"), subDivisions_); // <-- not const, updated on frame
	shader_.unuse();

	GL_CHECK_ERRORS;

	// Create geometry...
	vertices_[0] = glm::vec3(-5, 0, -5);
	vertices_[1] = glm::vec3(-5, 0, 5);
	vertices_[2] = glm::vec3(5, 0, 5);
	// vertices[3] = glm::vec3(5, 0, -5); // IT ALSO WORKS WITH ONLY A TRIANGLE

	// ...and topology
	GLushort *id = &indices_[0];
	id[0] = 0; id[1] = 1; id[2] = 2;
	//id[3] = 0; id[4] = 2; id[5] = 3; // IT ALSO WORKS WITH ONLY A TRIANGLE

	// Store geometry and topology in the object buffer(s) (VAO / VBO)
	glGenVertexArrays(1, &vaoID_);
	glGenBuffers(1, &vboVerticesID_);
	glGenBuffers(1, &vboIndicesID_);

	glBindVertexArray(vaoID_);

	// Pass the quad vertices to buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), &vertices_[0], GL_STATIC_DRAW);
	GL_CHECK_ERRORS;

	// Enable vertex attribute array for position
	glEnableVertexAttribArray(shader_["vVertex"]);
	glVertexAttribPointer(shader_["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	GL_CHECK_ERRORS;

	// Pass the quad indices to element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), &indices_[0], GL_STATIC_DRAW);
	GL_CHECK_ERRORS;

	// Set the poly mode to render lines (wireframe rendering)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GL_CHECK_ERRORS;

	std::cout << "Initialization successful\n";
}

void SubdivisionEngine::onRender()
{
	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the camera transformation
	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist_));
	glm::mat4 Rx = glm::rotate(T, rX_, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 MV = glm::rotate(Rx, rY_, glm::vec3(0.0f, 1.0f, 0.0f));

	shader_.use();
	glUniform1i(shader_("sub_divisions"), subDivisions_);

	// 1st submesh
	MV = glm::translate(MV, glm::vec3(-6, 0, -6));
	glUniformMatrix4fv(shader_("MVP"), 1, GL_FALSE, glm::value_ptr(P_ * MV));
	glDrawElements(GL_TRIANGLES, TotalIndices, GL_UNSIGNED_SHORT, nullptr);

	// 2nd submesh
	MV = glm::translate(MV, glm::vec3(12, 0, 0));
	glUniformMatrix4fv(shader_("MVP"), 1, GL_FALSE, glm::value_ptr(P_ * MV));
	glDrawElements(GL_TRIANGLES, TotalIndices, GL_UNSIGNED_SHORT, nullptr);

	// 3rd submesh
	MV = glm::translate(MV, glm::vec3(0, 0, 12));
	glUniformMatrix4fv(shader_("MVP"), 1, GL_FALSE, glm::value_ptr(P_ * MV));
	glDrawElements(GL_TRIANGLES, TotalIndices, GL_UNSIGNED_SHORT, nullptr);

	// 4th submesh
	MV = glm::translate(MV, glm::vec3(-12, 0, 0));
	glUniformMatrix4fv(shader_("MVP"), 1, GL_FALSE, glm::value_ptr(P_ * MV));
	glDrawElements(GL_TRIANGLES, TotalIndices, GL_UNSIGNED_SHORT, nullptr);

	shader_.unuse();

	glutSwapBuffers();
}

void SubdivisionEngine::onResize(int w, int h)
{
	// Set viewport size
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

	// Setup the projection matrix
	P_ = glm::perspective(45.0f, static_cast<GLfloat>(w) / h, 0.01f, 10000.0f);
}

void SubdivisionEngine::onShutdown()
{
	// Destroy shader
	shader_.deleteShaderProgram();

	// Destroy VAO and VBO
	glDeleteBuffers(1, &vboVerticesID_);
	glDeleteBuffers(1, &vboIndicesID_);
	glDeleteVertexArrays(1, &vaoID_);

	std::cout << "Shutdown successful\n";
}

void SubdivisionEngine::onMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN) oldX_ = x, oldY_ = y;
	state_ = button == GLUT_RIGHT_BUTTON;
}

void SubdivisionEngine::onMouseMove(int x, int y)
{
	if (state_)
		dist_ *= 1 + (y - oldY_) / 60.0f;
	else {
		rY_ += (x - oldX_) / 30.0f;
		rX_ += (y - oldY_) / 20.0f;
	}
	oldX_ = x; oldY_ = y;
	glutPostRedisplay();
}

void SubdivisionEngine::onKey(unsigned char key, int, int)
{
	switch (key) {
	case ',': subDivisions_--; break;
	case '.': subDivisions_++; break;
	}
	subDivisions_ = std::max(0, std::min(8, subDivisions_));
	glutPostRedisplay();
}
