#include "1_GettingStarted.h"

using namespace std;

void GettingStartedEngine::onInit()
{
	glClearColor(1.0f, 0.7f, 0, 0); // Alpha channel only used in alpha blending
	cout << "Initialization successful\n";
}

void GettingStartedEngine::onRender()
{
	// Clear color and depth buffer to the clear color and clear depth values.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Current back buffer is set as the current front buffer, shown on the screen.
	// Required in a double buffered OpenGL application.
	glutSwapBuffers();
}

void GettingStartedEngine::onResize(int w, int h)
{
	cout << "Size: " << w << ' ' << h << '\n';
}

void GettingStartedEngine::onShutdown()
{
	cout << "Shutdown successful\n";
}
