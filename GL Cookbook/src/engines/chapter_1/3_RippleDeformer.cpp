#include "3_RippleDeformer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float
	RippleDeformerEngine::SizeX = 4,
	RippleDeformerEngine::SizeZ = 4,
	RippleDeformerEngine::Speed = 2;

RippleDeformerEngine::RippleDeformerEngine() : GLEngine(GLE_REGISTER_ALL ^ GLE_REG_KEY)
{
	// Generate and pass mesh vertices to geometry
	mesh_.genBufferData<Vertex>(GL_ARRAY_BUFFER, TotalVertices, GL_STATIC_DRAW, [](Vertex *p)
	{
		// Create the geometry and topology
		//int count = 0;
		for (int j = 0; j <= NumZ; ++j) {
			for (int i = 0; i <= NumX; ++i) {
				// HE USED (float)i / (NUM_X - 1), probably intending for (..; < NUM_X; ..),
				// but this is really 0 to 1. Also used count++ as index
				p[i + (NumX + 1) * j] = glm::vec3(
					// (0 to 1 range) -> (-1 to 1 range) -> {-HALF_SIZE to HALF_SIZE range)
					(static_cast<float>(i) / NumX * 2 - 1) * (SizeX / 2), 0,
					(static_cast<float>(j) / NumZ * 2 - 1) * (SizeZ / 2));
			}
		}
	});

	// Generate and pass indices to geometry
	mesh_.genBufferData<GLushort>(GL_ELEMENT_ARRAY_BUFFER, TotalIndices, GL_STATIC_DRAW, [](GLushort *p)
	{
		// Fill the plane indices array
		GLushort *id = &p[0];
		for (int i = 0; i < NumZ; ++i) {
			for (int j = 0; j < NumX; ++j) {
				int i0 = i * (NumX + 1) + j;	// square top-left
				int i1 = i0 + 1;				// square top-right
				int i2 = i0 + (NumX + 1);		// square bottom-left
				int i3 = i2 + 1;				// square bottom-right
				if ((j + i) % 2) { // even iteration
					*id++ = i0; *id++ = i2; *id++ = i1; // diagonal '\'
					*id++ = i1; *id++ = i2; *id++ = i3;
				}
				else { // odd iteration
					*id++ = i0; *id++ = i2; *id++ = i3; // diagonal '/'
					*id++ = i0; *id++ = i3; *id++ = i1;
				}
			}
		}
	});

	/* Caution
	 *   1. The first parameter can only be 'GL_FRONT_AND BACK' in the core profile.
	 *   2. Make sure to use 'GL_LINE' instead of 'GL_LINES', which is used with the 'glDraw'
	 *      functions; to disable wireframe rendering, replace the second parameter with 'GL_FILL'. */
	// TODO: Apply polygon mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GL_CHECK_ERRORS;

	// Load the shader using the 'GLSLShader' class
	shader_.loadFromFile(GL_VERTEX_SHADER, "shaders/a3_ripple_vert.glsl");
	shader_.loadFromFile(GL_FRAGMENT_SHADER, "shaders/a3_ripple_frag.glsl");
	shader_.createAndLinkProgram();

	// Add shader attributes and uniforms
	shader_.use();
	shader_.addAttribute("vVertex");
	shader_.addUniform("MVP");
	shader_.addUniform("time");
	shader_.unuse();
	GL_CHECK_ERRORS;

	// Enable vertex attribute array for position
	glEnableVertexAttribArray(shader_["vVertex"]);
	glVertexAttribPointer(shader_["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	GL_CHECK_ERRORS;

	std::cout << "Initialization successful\n";
}

void RippleDeformerEngine::onRender()
{
	time_ = glutGet(GLUT_ELAPSED_TIME) / 1000.0f * Speed; // Get current time to animate

	// Set the camera viewing transformation
	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist_)); // Traslate Z
	glm::mat4 Rx = glm::rotate(T, rX_, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate X
	glm::mat4 MV = glm::rotate(Rx, rY_, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate Y
	glm::mat4 MVP = P_ * MV; // 'P_' is calculated in 'onResize()'

	// Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind the 'GLSLShader' shader, pass the uniforms and then draw the geometry.
	shader_.use();
	// Pass uniforms
	glUniformMatrix4fv(shader_("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform1f(shader_("time"), time_);
	// Attributes transferred now: on mesh triangles drawing call
	glDrawElements(GL_TRIANGLES, TotalIndices, GL_UNSIGNED_SHORT, nullptr);
	shader_.unuse();

	// Swap front and back buffers to show the rendered result
	glutSwapBuffers();
}

void RippleDeformerEngine::onResize(int w, int h)
{
	// Set the viewport size
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

	// Setup the projection matrix: Perspective
	// (vertical fov, aspect ratio, near and far clip distances)
	P_ = glm::perspective(45.0f, static_cast<GLfloat>(w) / h, 1.0f, 1000.0f);
}

void RippleDeformerEngine::onShutdown()
{
	// Destroy shader
	shader_.deleteShaderProgram();

	// VAO and VBOs are destroyed by the geometry object destructor

	std::cout << "Shutdown successful\n";
}

void RippleDeformerEngine::onMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN) oldX_ = x, oldY_ = y;
	state_ = (button != GLUT_RIGHT_BUTTON); // 'cause I don't have mid. button
}

void RippleDeformerEngine::onMouseMove(int x, int y)
{
	if (!state_) // Calculate distance if right mouse button
		dist_ *= (1 + (y - oldY_) / 60.0f);
	else {
		rY_ += (x - oldX_) / 30.0f;
		rX_ += (y - oldY_) / 20.0f;
	}
	oldX_ = x, oldY_ = y; // Update oldX and oldY for next event.
	glutPostRedisplay(); // Request freeglut to repaint application window.
}

void RippleDeformerEngine::onIdle()
{
	glutPostRedisplay();
}
