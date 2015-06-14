#include "6_ImageLoader.h"

#include <string>

#include <SOIL.h>
#pragma comment(lib, "SOIL.lib")

ImageLoaderEngine::ImageLoaderEngine(const char* texturePath)
	: GLEngine(GLE_REGISTER_DISPLAY), texturePath_(texturePath)
{
	GL_CHECK_ERRORS;

	// Load, compile and link shader
	shader_.loadFromFile(GL_VERTEX_SHADER, "shaders/a6_image_vert.glsl");
	shader_.loadFromFile(GL_FRAGMENT_SHADER, "shaders/a6_image_frag.glsl");
	shader_.createAndLinkProgram();

	// Add attributes and uniforms
	shader_.use();
	shader_.addAttribute("vVertex");
	shader_.addUniform("textureMap");
	glUniform1i(shader_("textureMap"), 0); // Pass const value of texture unit 0 (GL_TEXTURE0)
	shader_.unuse();

	GL_CHECK_ERRORS;

	geom_.genBufferData<glm::vec2>(GL_ARRAY_BUFFER, TotalVertices, GL_STATIC_DRAW, [](glm::vec2 *p)
	{
		// Setup geometry and topology
		p[0] = glm::vec2(0.0, 0.0);
		p[1] = glm::vec2(1.0, 0.0);
		p[2] = glm::vec2(1.0, 1.0);
		p[3] = glm::vec2(0.0, 1.0);
	});
	GL_CHECK_ERRORS;

	geom_.genBufferData<GLushort>(GL_ELEMENT_ARRAY_BUFFER, TotalIndices, GL_STATIC_DRAW, [](GLushort *p)
	{
		p[0] = 0; p[1] = 1; p[2] = 2;
		p[3] = 0; p[4] = 2; p[5] = 3;
	});
	GL_CHECK_ERRORS;

	// Enable vertex attribute array for position
	glEnableVertexAttribArray(shader_["vVertex"]);
	glVertexAttribPointer(shader_["vVertex"], 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	GL_CHECK_ERRORS;

	loadTexture();
	GL_CHECK_ERRORS;

	std::cout << "Initialization successful\n";
}

void ImageLoaderEngine::onRender()
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind the shader, draw the fullscreen quad and unbind
	shader_.use();
	glDrawElements(GL_TRIANGLES, TotalIndices, GL_UNSIGNED_SHORT, nullptr);
	shader_.unuse();

	// Swap front and back buffers to show the rendered result
	glutSwapBuffers();
}

void ImageLoaderEngine::onResize(int w, int h)
{
	// Set the viewport
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
}

void ImageLoaderEngine::onShutdown()
{
	// Destroy shader
	shader_.deleteShaderProgram();

	// VAO and VBOs are destroyed by the geometry object destructor

	// Delete textures too
	glDeleteTextures(1, &textureID_);

	std::cout << "Shutdown successful\n";
}

void ImageLoaderEngine::loadTexture()
{
	// Load the image using SOIL
	int textureWidth = 0, textureHeight = 0, textureChannels = 0;
	// Here, 'SOIL_LOAD_AUTO' keeps all loading settings to default
	GLubyte *pData = SOIL_load_image(texturePath_.c_str(),
		&textureWidth, &textureHeight, &textureChannels, SOIL_LOAD_AUTO);

	if (pData == nullptr) { // If it fails, returns NULL
		std::cerr << "Cannot load image: " << texturePath_ << std::endl;
		assert(pData != nullptr, "Cannot load texture image");
		exit(EXIT_FAILURE);
	}

	// Vertically flip the image on Y axis since it is inverted
	for (int j = 0; 2 * j < textureHeight; ++j) {
		int index1 = j * textureWidth * textureChannels;
		int index2 = (textureHeight - 1 - j) * textureWidth * textureChannels;
		for (int i = textureWidth * textureChannels; i > 0; --i) {
			std::swap(pData[index1], pData[index2]);
			++index1, ++index2;
		}
	}

	// Set up the OpenGL texture object and bind to texture unit 0
	glGenTextures(1, &textureID_);
	glActiveTexture(GL_TEXTURE0); // Set active texture unit to 0
	glBindTexture(GL_TEXTURE_2D, textureID_);

	// Set texture parameters
	//   filtering for minification / magnification and wrapping modes for S and T coordinates
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Allocate texture
	//   (target, mipmap, internal_fmt, w, h, border, image_fmt, ptr_fmt, data)
	//   internal_fmt by image properties
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);

	// Free SOIL image data
	SOIL_free_image_data(pData);
}
