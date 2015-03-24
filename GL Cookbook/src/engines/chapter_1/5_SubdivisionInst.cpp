#include "5_SubdivisionInst.h"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SubdivisionInstEngine::SubdivisionInstEngine() : GLEngine(GLE_REGISTER_ALL ^ GLE_REG_IDLE)
{
	GL_CHECK_ERRORS;

	// Temporary snippet to show maximum geometry shader output.
	// Since my laptop's graphics card isn't fully compliant to OpenGL 3.3, this
	// doesn't work, at least for me. However, the whole test runs fine.
	//   GLint maxVerts;
	//   glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &maxVerts);
	//   if (glGetError() == GL_NO_ERROR)
	//     cout << "Max. geometry shader output : " << maxVerts << '\n';
	//   else // GL_INVALID_ENUM
	//     cout << "Cannot retrieve GL_MAX_GEOMETRY_OUTPUT_VERTICES (min. GL version: 3.2).\n";

	// Load, create and link the shader
	shader_.loadFromFile(GL_VERTEX_SHADER, "shaders/a5_subdivi_vert.glsl");
	shader_.loadFromFile(GL_GEOMETRY_SHADER, "shaders/a5_subdivi_geom.glsl");
	shader_.loadFromFile(GL_FRAGMENT_SHADER, "shaders/a4_subdiv_frag.glsl");
	shader_.createAndLinkProgram();

	M_[0] = translate(glm::mat4(1), glm::vec3(-6, 0, -6));
	M_[1] = translate(M_[0], glm::vec3(12, 0, 0));
	M_[2] = translate(M_[1], glm::vec3(0, 0, 12));
	M_[3] = translate(M_[2], glm::vec3(-12, 0, 0));

	// Add attributes and uniforms
	shader_.use();
	shader_.addAttribute("vVertex");
	shader_.addUniform("PV"); // New instanced attribs
	shader_.addUniform("M");
	shader_.addUniform("sub_divisions");
	glUniform1i(shader_("sub_divisions"), subDivisions_); // <-- set initial value here
	glUniformMatrix4fv(shader_("M"), 4, GL_FALSE, glm::value_ptr(M_[0]));
	shader_.unuse();

	GL_CHECK_ERRORS;

	// Generate and pass the quad vertices to buffer object
	geom_.genStaticBufferData<glm::vec3>(GL_ARRAY_BUFFER, TotalVertices, [](glm::vec3 *p)
	{
		// Create geometry
		p[0] = glm::vec3(-5, 0, -5);
		p[1] = glm::vec3(-5, 0, 5);
		p[2] = glm::vec3(5, 0, 5);
		//vertices_[3] = glm::vec3(5, 0, -5); // IT ALSO WORKS WITH ONLY A TRIANGLE
	});
	GL_CHECK_ERRORS;

	// Generate and pass the quad indices to element array buffer
	geom_.genStaticBufferData<GLushort>(GL_ELEMENT_ARRAY_BUFFER, TotalIndices, [](GLushort *p)
	{
		// ...and topology
		p[0] = 0; p[1] = 1; p[2] = 2;
		//p[3] = 0; p[4] = 2; p[5] = 3; // IT ALSO WORKS WITH ONLY A TRIANGLE
	});
	GL_CHECK_ERRORS;

	// Enable vertex attribute array for position
	glEnableVertexAttribArray(shader_["vVertex"]);
	glVertexAttribPointer(shader_["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	GL_CHECK_ERRORS;

	// Set the poly mode to render lines (wireframe rendering)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GL_CHECK_ERRORS;

	std::cout << "Initialization successful\n";
}

void SubdivisionInstEngine::onRender()
{
	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the camera transformation
	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist_));
	glm::mat4 Rx = glm::rotate(T, rX_, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 V = glm::rotate(Rx, rY_, glm::vec3(0.0f, 1.0f, 0.0f)); // ex. MV
	glm::mat4 PV = P_ * V;

	shader_.use();
	// glUniform1i(shader_("sub_divisions"), subDivisions_);	Already handled on init and key event
	glUniformMatrix4fv(shader_("PV"), 1, GL_FALSE, glm::value_ptr(PV));
	glDrawElementsInstanced(GL_TRIANGLES, TotalIndices, GL_UNSIGNED_SHORT, nullptr, 4);
	shader_.unuse();

	glutSwapBuffers();
}

void SubdivisionInstEngine::onResize(int w, int h)
{
	// Set the viewport size
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

	// Setup the projection matrix
	P_ = glm::perspective(45.0f, static_cast<GLfloat>(w) / h, 0.01f, 10000.0f);
}

void SubdivisionInstEngine::onShutdown()
{
	// Destroy shader
	shader_.deleteShaderProgram();

	// VAO and VBOs are destroyed by the geometry object destructor

	std::cout << "Shutdown successful\n";
}

void SubdivisionInstEngine::onMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN) oldX_ = x, oldY_ = y;
	state_ = button == GLUT_RIGHT_BUTTON;
}

void SubdivisionInstEngine::onMouseMove(int x, int y)
{
	if (state_)
		dist_ *= 1 + (y - oldY_) / 60.0f;
	else {
		rY_ += (x - oldX_) / 30.0f;
		rX_ += (y - oldY_) / 20.0f;
	}
	oldX_ = x, oldY_ = y;
	glutPostRedisplay();
}

void SubdivisionInstEngine::onKey(unsigned char key, int, int)
{
	switch (key) {
	case ',': subDivisions_--; break;
	case '.': subDivisions_++; break;
	}
	subDivisions_ = std::max(0, std::min(8, subDivisions_));
	shader_.use();
	glUniform1i(shader_("sub_divisions"), subDivisions_); // <-- update value
	shader_.unuse();
	glutPostRedisplay();
}
