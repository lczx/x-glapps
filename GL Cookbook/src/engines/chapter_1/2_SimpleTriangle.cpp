#include "2_SimpleTriangle.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SimpleTriangleEngine::SimpleTriangleEngine() : GLEngine(GLE_REGISTER_DISPLAY)
{
	GL_CHECK_ERRORS;

	// - VAO already set up in 'Geometry' constructor),
	//   will be bound automatically when calling 'Geometry' methods.
	// - VBOs will be generated automatically when assigning data
	//   on 'addStaticBufferData' and 'genStaticBufferData'

	/* The 'Geometry' object will bind the buffer object storing vertices with the passed
	* in binding point (GL_ARRAY_BUFFER). Here we pass a generator for the points that
	* will be passed in to the buffer using the same binding. We pass also the number of
	* elements and the vertex type. The 'Geometry' will use 'GL_STATIC_DRAW',
	* that means that we are not going to modify the data often. */

	// Generate and pass triangle vertices to buffer object
	triangle_.genBufferData<VertexEx>(GL_ARRAY_BUFFER, 3, GL_STATIC_DRAW, [](VertexEx *p)
	{
		// Setup triangle geometry: vertices(position, color)
		p[0] = { glm::vec3(-1, -1, 0),	glm::vec3(1, 0, 0) };
		p[1] = { glm::vec3(0, 1, 0),	glm::vec3(0, 1, 0) };
		p[2] = { glm::vec3(1, -1, 0),	glm::vec3(0, 0, 1) };
	});
	GL_CHECK_ERRORS;

	// Generate and pass indices to element array buffer
	triangle_.genBufferData<GLushort>(GL_ELEMENT_ARRAY_BUFFER, 3, GL_STATIC_DRAW, [](GLushort *p)
	{
		// Setup triangle geometry: indices
		p[0] = 0;
		p[1] = 1;
		p[2] = 2;
	});
	GL_CHECK_ERRORS;

	// Load shader objects
	shader_.loadFromFile(GL_VERTEX_SHADER, "shaders/a2_triangle_vert.glsl");
	shader_.loadFromFile(GL_FRAGMENT_SHADER, "shaders/a2_triangle_frag.glsl");

	// Compile and link shader program
	shader_.createAndLinkProgram();

	// Add attributes and uniforms
	shader_.use();
	shader_.addAttribute("vVertex");
	shader_.addAttribute("vColor");
	shader_.addUniform("MVP");
	shader_.unuse();

	GL_CHECK_ERRORS;

	/* Enable vertex attributes on the shader
	* (attrib. location, elements amount, type, is_normalized, stride, offset) */

	GLsizei stride = sizeof(VertexEx); // num. of bytes to jump to reach next elem. of same attribute.

	// Enable vertex attribute array for position
	glEnableVertexAttribArray(shader_["vVertex"]);
	glVertexAttribPointer(shader_["vVertex"], 3, GL_FLOAT, GL_FALSE, stride, nullptr);
	GL_CHECK_ERRORS;

	// Enable vertex attribute array for color
	glEnableVertexAttribArray(shader_["vColor"]);
	glVertexAttribPointer(shader_["vColor"], 3, GL_FLOAT, GL_FALSE, stride,
		reinterpret_cast<const GLvoid *>(offsetof(VertexEx, color)));
	GL_CHECK_ERRORS;

	std::cout << "Initialization successful\n";
}

void SimpleTriangleEngine::onRender()
{
	// Clear color and depth buffer to the 'clear color' and 'clear depth' values.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind the shader
	shader_.use();

	/* 'glm::value_ptr' gets the float pointer from the matrix object.
	 * Note that the OpenGL matrices are concatenated right to left since it follows a
	 * right handed coordinate system in a column major layout. Hence we keep
	 * the projection matrix on the left and the modelview matrix on the right.
	 * For this simple example, the modelview (MV) is the identity matrix. */

	// Pass in the shader uniform (combined modelview and projection matrix)
	// (uniform location, number of matrices to pass, needs_to_be_transposed, float ptr. to matrix obj.)
	glUniformMatrix4fv(shader_("MVP"), 1, GL_FALSE, glm::value_ptr(P_ * MV_));

	/* Notice that we have left our VAO bound and we will pass '0' (nullptr) as last parameter.
	 * This tells the GPU to use the references of the GL_ELEMENT_ARAY_BUFFER and GL_ARRAY_BUFFER
	 * binding points of the bound VAO. Thus we don't need to explicitly bind the 'vboVerticesID'
	 * and 'vboIndicesID' buffer objects again. */

	// Draw triangle
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);

	// Unbind the shader
	shader_.unuse();

	// SWAP DAT BUFFER AND SHOW RESULT 2 DA PEEPLE!
	glutSwapBuffers();
}

void SimpleTriangleEngine::onResize(int w, int h)
{
	// Set the viewport size
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

	// Setup the projection matrix: Orthographic projection
	// view volume -> (-1, 1, -1, 1) (left, right, bottom, top)
	P_ = glm::ortho(-1, 1, -1, 1);
}

void SimpleTriangleEngine::onShutdown()
{
	// Destroy shader
	// If we don't delete shader programs we will have graphic memory leaks
	shader_.deleteShaderProgram();

	// VAO and VBOs are destroyed by the geometry object destructor

	std::cout << "Shutdown successful\n";
}
