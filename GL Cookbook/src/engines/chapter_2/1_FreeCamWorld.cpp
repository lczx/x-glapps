#include "1_FreeCamWorld.h"

#define _USE_MATH_DEFINES
#include <math.h> /* M_PI */

#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#pragma comment(lib, "SOIL.lib")

using namespace std;

// For floating point imprecision
const float Epsilon = 0.001f;
const float Epsilon2 = Epsilon * Epsilon;

// Virtual key codes
const int
	VK_W = 0x57,
	VK_S = 0x53,
	VK_A = 0x41,
	VK_D = 0x44,
	VK_Q = 0x51,
	VK_Z = 0x5A;

// LOCAL CHECKERBOARD TEXTURE LOADER
void genCheckerboardTexture()
{
	// Generate the checker texture
	//? This code may be faster if we iterate over 'i' first.
	GLubyte data[128][128] = { 0 };
	for (int j = 0; j < 128; ++j)
		for (int i = 0; i < 128; ++i)
			data[i][j] = (i <= 64 && j <= 64 || i > 64 && j > 64) ? 255 : 0;

	// Allocate texture object
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 128, 128, 0, GL_RED, GL_UNSIGNED_BYTE, data);
}

// LOCAL FANCY TEXTURE LOADER
//! (try to reuse ImageLoaderEngine::LoadTexture())
//  watch out at texture inversion and fix clockwise vertices positioning there)
void genFancyTexture()
{
	string txPath = "assets/fancytexture.png";

	int txW = 0, txH = 0, txCh = 0;
	GLubyte *pData = SOIL_load_image(txPath.c_str(), &txW, &txH, &txCh, SOIL_LOAD_AUTO);
	// cout << txW << ' ' << txH << ' ' << txCh << '\n';

	if (!pData) {
		cerr << "Cannot load image: " << txPath.c_str() << endl;
		// No exit, so we can see the error...
	}

	// IMAGE IS NOT FLIPPED!

	// If using texture with alpha (level in photoshop), use GL_RGBA instead
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, txW, txH, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);

	// Free SOIL image data
	SOIL_free_image_data(pData);
}

// Initialize OpenGL
void FreeCamWorld::onInit()
{
	GL_CHECK_ERRORS;

	// Generate texture object
	glGenTextures(1, &checkerTextureID_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, checkerTextureID_);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	GL_CHECK_ERRORS;

	// Set maximum anisotropy setting
	GLfloat largestSupportedAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largestSupportedAnisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largestSupportedAnisotropy);

	// Set mipmap base and max level
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

	// Generate and allocate texture object
	genFancyTexture();

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	GL_CHECK_ERRORS;

	// Create a textured plane object
	pCheckerPlane_ = new TexturedPlane();
	GL_CHECK_ERRORS;

	// Setup camera: position and look direction
	glm::vec3 p = glm::vec3(5);
	cam.setPosition(p);
	glm::vec3 look = glm::normalize(p);

	// Setup camera: rotate it for proper orientation
	rX_ = glm::degrees(float(atan2(look.z, look.x) + M_PI)); /* yaw */
	rY_ = glm::degrees(asin(look.y)); /* pitch */

	mouseFilter << glm::vec2(rX_, rY_);
	mouseFilter.setEnabled(true);

	// On init, filtered mouse output is already rX_ and rY_.
	cam.rotate(rX_, rY_, 0);
	cout << "Initialization successful\n";
}

// Display callback function
void FreeCamWorld::onRender()
{
	// Timing related calculation
	lastTime_ = currentTime_;
	currentTime_ = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	dt_ = currentTime_ - lastTime_;

	// Clear color buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the camera transformation
	glm::mat4 MV = cam.getViewMatrix();
	glm::mat4 P = cam.getProjectionMatrix();
	glm::mat4 MVP = P * MV;

	// Render the checkered plane
	pCheckerPlane_->render(glm::value_ptr(MVP));

	// Swap front and back buffers to show the rendered result
	glutSwapBuffers();
}

// Resize event handler
void FreeCamWorld::onResize(int w, int h)
{
	// Set the viewport
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

	// Setup the camera projection matrix
	cam.setupProjection(45, static_cast<GLfloat>(w) / h);
}

// Delete all allocated resources
void FreeCamWorld::onShutdown()
{
	delete pCheckerPlane_;
	glDeleteTextures(1, &checkerTextureID_);
	cout << "Shutdown successful\n";
}

// Mouse click handler
void FreeCamWorld::onMouseDown(int button, int s, int x, int y)
{
	//! Update this code as in other samples (and everywhere else)
	if (s == GLUT_DOWN) { oldX_ = x; oldY_ = y; }
	state_ = button == GLUT_MIDDLE_BUTTON;
}

// Mouse move handler
void FreeCamWorld::onMouseMove(int x, int y)
{
	if (state_) {
		// If zoom state, calculate fov and setup camera projection matrix
		fov_ += (y - oldY_) / 5.0f;
		cam.setupProjection(fov_, cam.getAspectRatio());
	} else {
		// If rotate state, calculate the rotation amount (pitch and yaw).
		// If mouse filtering is enabled, use the filtered mouse input,
		// otherwise use the default rotation amount.

		// It is always better to use filtered mouse input, which gives smoother movement.
		rY_ += (y - oldY_) / 5.0f;
		rX_ += (oldX_ - x) / 5.0f;

		glm::vec2 mousePos = (mouseFilter << glm::vec2(rX_, rY_)).getCurrentMousePos();
		cam.rotate(mousePos.x, mousePos.y, 0);
	}
	oldX_ = x;
	oldY_ = y;

	glutPostRedisplay();
}

// Keyboard event handler to toggle the mouse filtering using spacebar key
void FreeCamWorld::onKey(unsigned char key, int, int)
{
	if (key == ' ') {
		mouseFilter.setEnabled(!mouseFilter.isEnabled());
		cout << "Filtering " << (mouseFilter.isEnabled() ? "enabled" : "disabled") << '\n';
	}
	glutPostRedisplay();
}

// Idle event processing
void FreeCamWorld::onIdle()
{
	// Handle the WASD, QZ key events to move the camera around

	if (GetAsyncKeyState(VK_W) & 0x8000) cam.walk(dt_);
	if (GetAsyncKeyState(VK_S) & 0x8000) cam.walk(-dt_);

	if (GetAsyncKeyState(VK_A) & 0x8000) cam.strafe(-dt_);
	if (GetAsyncKeyState(VK_D) & 0x8000) cam.strafe(dt_);

	if (GetAsyncKeyState(VK_Q) & 0x8000) cam.lift(dt_);
	if (GetAsyncKeyState(VK_Z) & 0x8000) cam.lift(-dt_);

	// This gradually decays the translation vector, otherwise we uncomment
	// the line in the 'update' method of the camera after translating its position.
	glm::vec3 t = cam.getTranslation();
	if (glm::dot(t, t) > Epsilon2) cam.setTranslation(t * .95f);

	// Call the display function
	glutPostRedisplay();
}
