#include "GLApplication.h"

#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

#pragma comment(lib, "glew32.lib")

using namespace std;

int GLApplication::glMajorVersion;
int GLApplication::glMinorVersion;

void GLApplication::initializeGLContext(int *argcp, char** argv, int glMajorVersion, int glMinorVersion)
{
	GLApplication::glMajorVersion = glMajorVersion;
	GLApplication::glMinorVersion = glMinorVersion;

	// Initialize GLUT environment
	glutInit(argcp, argv);

	// Initialize display mode with support for depth buffer, double buffering
	// (front and back buffer for smooth and flicker-free rendering), and RGBA colors.
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// Required OpenGL context and its flags
	// In OpenGL 4.3 we can register a callback when OpenGL related errors occur.
	// Passing 'GLUT_DEBUG' creates the context in debug mode, thus allowing the callback.
	glutInitContextVersion(glMajorVersion, glMinorVersion);

	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
}

void GLApplication::createWindow(int width, int height)
{
	// Set screen size and create window
	glutInitWindowSize(width, height);

	string title = engineName_ + " - OpenGL " +
		to_string(glMajorVersion) + '.' + to_string(glMinorVersion);
	glutCreateWindow(title.c_str());

	glewInitialize_();

	hookClass_->onInit(); // Renderere initialization
	hookClass_->registerHooks(); // Register renderer callbacks for window
}

void GLApplication::glewShowInfo()
{
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << '\n';
	cout << "  Vendor:   " << glGetString(GL_VENDOR) << '\n';
	cout << "  Renderer: " << glGetString(GL_RENDERER) << '\n';
	cout << "  Version:  " << glGetString(GL_VERSION) << '\n';
	cout << "  GLSL:     " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";

	GL_CHECK_ERRORS;
}

void GLApplication::loop()
{
	glutMainLoop();
}

void GLApplication::glewInitialize_()
{
	// This allows GLEW library to report hardware supported
	// extensions but not supported by experimental or pre-release drivers.
	glewExperimental = GL_TRUE;

	// Initialize GLEW library AFTER the OpenGL context has been created
	GLenum err = glewInit();
	if (err != GLEW_OK)
		cerr << "Error: " << glewGetErrorString(err) << '\n';
	//else if (GLEW_VERSION_3_3)
	//	cout << "Driver supports OpenGL 3.3\n";

	// err = glGetError(); // This is to ignore INVALID ENUM error 1282
	// TODO check if this is real

	GL_CHECK_ERRORS;
}